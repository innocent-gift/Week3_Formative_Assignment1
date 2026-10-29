def analyze_data(data, threshold):
    if not data: raise ValueError("Empty")
    n = len(data)
    mean = sum(data) / n
    return {"mean": mean, "min": min(data), "max": max(data)}
