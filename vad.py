import numpy as np
import librosa
import sounddevice as sd
import tensorflow as tf
import time

SAVED_MODEL_PATH = '/Users/aiden/mymodelplzwork/plz_model.h5'
SAMPLES_TO_CONSIDER = 22050  # 1 second of audio at 22.05kHz

class _Keyword_Spotting_Service:
    model = None
    _mapping = [
        "right",
        "left",
        "down",
        "up",
    ]
    _instance = None

    def __init__(self):
        self.model = tf.keras.models.load_model(SAVED_MODEL_PATH)

    def predict(self, mfcc):
        # we need a 4-dim array to feed to the model for prediction: (# samples, # time steps, # coefficients, 1)
        mfcc = mfcc[np.newaxis, ..., np.newaxis]

        # get the predicted label
        predictions = self.model.predict(mfcc)
        predicted_index = np.argmax(predictions)
        predicted_keyword = self._mapping[predicted_index]
        return predicted_keyword

    def preprocess(self, signal, sample_rate, num_mfcc=13, n_fft=2048, hop_length=512):
        # ensure consistency of the length of the signal
        if len(signal) >= SAMPLES_TO_CONSIDER:
            signal = signal[:SAMPLES_TO_CONSIDER]

            # extract MFCCs
            mfccs = librosa.feature.mfcc(y=signal, sr=sample_rate, n_mfcc=num_mfcc, n_fft=n_fft, hop_length=hop_length)
            return mfccs.T
        else:
            raise ValueError("Audio signal is too short")

# Singleton instance
def Keyword_Spotting_service():
    if _Keyword_Spotting_Service._instance is None:
        _Keyword_Spotting_Service._instance = _Keyword_Spotting_Service()
    return _Keyword_Spotting_Service._instance

def is_speech_present(signal, threshold=0.00001):
    # Calculate energy
    energy = np.sum(signal ** 2) / float(len(signal))
    
    # Compare with threshold
    if energy > threshold:
        return True
    else:
        return False

def record_audio(duration=1, fs=22050):
    print("Listening for speech...")
    while True:
        audio = sd.rec(int(duration * fs), samplerate=fs, channels=1, dtype='float32')
        sd.wait()
        print("Recording finished.")
        audio = np.squeeze(audio)  # Remove the channel dimension
        
        if is_speech_present(audio):
            return audio, fs
        else:
            print("No speech detected. Listening again...")

if __name__ == "__main__":
    # Get the keyword spotting service instance
    kss = Keyword_Spotting_service()

    while True:
        try:
            # Record audio
            signal, sample_rate = record_audio()

            # Preprocess the audio signal
            mfcc = kss.preprocess(signal, sample_rate)

            # Make a prediction
            predicted_keyword = kss.predict(mfcc)
            print(f"Predicted keyword: {predicted_keyword}")
            
            # Small delay to avoid excessive CPU usage
            time.sleep(0.5)
        except ValueError as e:
            print(e)
            # Small delay to avoid excessive CPU usage
            time.sleep(0.5)
