# AI-Powered DDoS Detection Server

This project is a Flask-based web application designed to monitor and predict potential DDoS (Distributed Denial-of-Service) attacks in real-time. It uses a pre-trained AI model to analyze incoming traffic data and provides insights into whether the traffic is normal or suspicious.

## Features

- **Real-Time Traffic Monitoring**: Tracks incoming requests and errors to detect unusual patterns.
- **AI-Powered Predictions**: Uses a machine learning model to classify traffic as "Normal" or "DDoS Attack Detected!".
- **Data Visualization**: Provides real-time data for graphing traffic trends.
- **Cross-Origin Support**: Allows seamless integration with other systems via CORS.
- **Error Handling**: Logs warnings and errors for debugging and ensures smooth operation.

## Endpoints

### `/`
- **Method**: `GET`
- **Description**: Serves the homepage (`index.html`).

### `/predict`
- **Method**: `POST`
- **Description**: Accepts JSON data with `requests` (number of requests) and `errors` (number of errors). Returns a prediction of traffic status.
- **Example Request**:
  ```json
  {
    "timestamps": ["12:00:01", "12:00:02"],
    "requests": [100, 120],
    "errors": [5, 10]
  }

Setup Instructions
Clone the Repository:

git clone https://github.com/your-repo/ai-ddos-detection.git
cd ai-ddos-detection

Install Dependencies: Ensure you have Python installed, then run:
pip install -r requirements.txt

Add the AI Model: Place the pre-trained model file (ddos_model.pkl) in the project directory.

Run the Application: Start the Flask server:
python plotter.py

Access the App: Open your browser and navigate to http://localhost:5000.

Requirements
Python 3.7+
Flask
Flask-CORS
Joblib
Pandas
Logging
The application logs important events, warnings, and errors to help with debugging and monitoring. Logs are displayed in the console.

Notes
Ensure the AI model (ddos_model.pkl) is trained and compatible with the input data format.
The app limits stored data to 50 points to prevent memory issues.
Future Improvements
Add a frontend for real-time graph visualization.
Enhance the AI model for better accuracy.
Implement user authentication for secure access.
License
This project is licensed under the MIT License. See the LICENSE file for details.

