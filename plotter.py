
from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
import joblib
import pandas as pd
import time
import logging

# Configure logging
logging.basicConfig(level=logging.DEBUG, 
                    format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

# Load AI Model
try:
    model = joblib.load("ddos_model.pkl")
    logger.info("AI Model Loaded Successfully!")
except Exception as e:
    logger.error(f"Error loading model: {e}")
    model = None

app = Flask(__name__)
# More permissive CORS settings
CORS(app, resources={r"/*": {"origins": "*"}})

# Global variables to store data
MAX_DATA_POINTS = 50
request_data = []
error_data = []
timestamps = []

@app.route('/')
def home():
    return render_template("index.html")

@app.route('/predict', methods=['POST', 'OPTIONS'])
def predict():
    # Handle CORS preflight requests
    if request.method == 'OPTIONS':
        return '', 204

    try:
        data = request.json
        if not data:
            logger.warning("No data received in predict endpoint")
            return jsonify({"error": "No data received"}), 400

        requests_count = data.get("requests", 0)
        errors_count = data.get("errors", 0)

        logger.debug(f"Received Data: Requests={requests_count}, Errors={errors_count}")

        # Truncate data to prevent memory issues
        if len(request_data) >= MAX_DATA_POINTS:
            request_data.pop(0)
            error_data.pop(0)
            timestamps.pop(0)

        # Convert to DataFrame for AI Model
        sample_data = pd.DataFrame([[requests_count, errors_count]], columns=["requests", "errors"])

        # AI Prediction
        prediction = model.predict(sample_data)[0] if model else 0
        status = " DDoS Attack Detected!" if prediction == 1 else "Normal Traffic"

        # Store data for real-time graph
        request_data.append(requests_count)
        error_data.append(errors_count)
        timestamps.append(time.strftime("%H:%M:%S"))

        return jsonify({
            "prediction": status, 
            "requests": requests_count, 
            "errors": errors_count
        })

    except Exception as e:
        logger.error(f"Error processing request: {e}", exc_info=True)
        return jsonify({"error": str(e)}), 500

@app.route('/graph', methods=['GET', 'OPTIONS'])
def graph():
    # Handle CORS preflight requests
    if request.method == 'OPTIONS':
        return '', 204

    if not request_data:
        logger.debug("No data available, adding dummy data")
        timestamps.append(time.strftime("%H:%M:%S"))
        request_data.append(0)
        error_data.append(0)

    logger.debug(f"Timestamps: {timestamps}")
    logger.debug(f"Requests: {request_data}")
    logger.debug(f"Errors: {error_data}")

    return jsonify({
        "timestamps": timestamps,
        "requests": request_data,
        "errors": error_data
    })
    

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)