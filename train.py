import pandas as pd
import joblib
from sklearn.ensemble import RandomForestClassifier

# Sample dataset (normal vs DDoS requests)
data = {
    "requests": [50, 100, 150, 500, 800, 1000, 1200, 1500],
    "errors": [2, 5, 10, 50, 80, 100, 150, 200],
    "attack": [0, 0, 0, 1, 1, 1, 1, 1]  # 1 = DDoS, 0 = Normal
}

# Convert to DataFrame
df = pd.DataFrame(data)

# Train AI Model
X = df[["requests", "errors"]]
y = df["attack"]
model = RandomForestClassifier(n_estimators=10, random_state=42)
model.fit(X, y)

# Save Model
joblib.dump(model, "ddos_model.pkl")    
print("🎯 AI Model Trained & Saved as 'ddos_model.pkl'!")
