import streamlit as st
import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import classification_report, confusion_matrix
from sklearn.preprocessing import LabelEncoder
import matplotlib.pyplot as plt
import seaborn as sns

# -------------------------------------------------------------
# Définition du churn :
# Le churn (attrition) désigne le fait qu'un client quitte l'entreprise,
# résilie son contrat ou arrête d'utiliser le service.
# Dans ce projet, la colonne 'churn' vaut 1 si le client est parti, 0 sinon.
# -------------------------------------------------------------

st.set_page_config(page_title="Churn Predictor", layout="wide")

st.title("\U0001F4CA Prédiction de Churn Client avec Random Forest")

st.markdown("""
**Qu'est-ce que le churn ?**

Le **churn** (ou attrition) désigne le fait qu'un client quitte l'entreprise, résilie son contrat ou arrête d'utiliser le service. 
Dans ce projet, la colonne `churn` vaut **1** si le client est parti, **0** s'il est resté.
""")

uploaded_file = st.file_uploader("Charge ton fichier Excel de clients", type=["xlsx"])

@st.cache_data
def load_data(file):
    return pd.read_excel(file)

def preprocess(df):
    df = df.copy()
    # Supprimer les identifiants clients
    if 'customer_id' in df.columns:
        df.drop('customer_id', axis=1, inplace=True)
    # Encoder les variables catégorielles
    for col in df.select_dtypes(include=['object']).columns:
        le = LabelEncoder()
        df[col] = le.fit_transform(df[col])
    return df

def train_model(X, y):
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
    model = RandomForestClassifier(n_estimators=200, max_depth=10, random_state=42)
    model.fit(X_train, y_train)
    y_pred = model.predict(X_test)
    report = classification_report(y_test, y_pred, output_dict=True)
    matrix = confusion_matrix(y_test, y_pred)
    return model, report, matrix, X_test

def show_metrics(report, matrix):
    st.subheader("✅ Rapport de Classification")
    df_report = pd.DataFrame(report).transpose()
    st.dataframe(df_report.style.background_gradient(cmap='RdYlGn'))

    st.subheader("⚖️ Matrice de Confusion")
    fig, ax = plt.subplots()
    sns.heatmap(matrix, annot=True, fmt="d", cmap="Blues", ax=ax)
    st.pyplot(fig)

    # Explication en français des résultats
    st.markdown("""
    ### Comment interpréter ces résultats ?
    - **Précision** : Parmi les clients prédits comme churn (ou non churn), combien sont corrects ?
    - **Rappel** : Parmi les vrais churn (ou non churn), combien ont été bien trouvés ?
    - **F1-score** : Moyenne harmonique entre précision et rappel (plus la valeur est proche de 1, mieux c'est).
    - **Support** : Nombre d'exemples de chaque classe dans le test.
    - **Accuracy** : Pourcentage global de bonnes prédictions.
    
    **Exemple d'interprétation :**
    - Si le rappel de la classe 1 (churn) est élevé, le modèle détecte bien les clients à risque.
    - Si la précision de la classe 1 est élevée, peu de clients sont faussement considérés comme à risque.
    - La matrice de confusion montre combien de clients ont été bien ou mal classés pour chaque catégorie.
    """)

if uploaded_file:
    df = load_data(uploaded_file)
    st.write("Aperçu des données:", df.head())

    df_processed = preprocess(df)
    X = df_processed.drop("churn", axis=1)
    y = df_processed["churn"]

    model, report, matrix, X_test = train_model(X, y)
    show_metrics(report, matrix)
else:
    st.info("Veuillez uploader un fichier Excel pour commencer.")
