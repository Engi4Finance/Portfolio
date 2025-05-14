# Project: Predictive analysis of customer churn

**Période :** janvier 2025 - février 2025  
**Associé à :** Icam - Institut Catholique d'Arts et Métiers

## Qu'est-ce que le churn ?
Le **churn** (ou attrition) désigne le fait qu'un client quitte l'entreprise, résilie son contrat ou arrête d'utiliser le service. 
Dans ce projet, la colonne `churn` vaut **1** si le client est parti, **0** s'il est resté.

## Description du projet
Ce projet propose une application interactive (Streamlit) permettant de prédire le risque de churn (départ) des clients à partir de leurs données. L'utilisateur peut charger un fichier Excel contenant les informations clients, le modèle prédit le churn et affiche des métriques d'évaluation (précision, rappel, f1-score, matrice de confusion) avec des explications en français pour faciliter l'interprétation.

## Fonctionnalités principales
- Upload d'un fichier Excel de clients
- Prétraitement automatique des données (encodage des variables catégorielles)
- Entraînement d'un modèle Random Forest
- Affichage du rapport de classification et de la matrice de confusion
- Explications détaillées en français sur l'interprétation des résultats

## Utilisation
1. Installer les dépendances :
   ```bash
   pip install -r requirements.txt
   ```
2. Lancer l'application Streamlit :
   ```bash
   streamlit run src/model.py
   ```
3. Uploader votre fichier Excel de clients (doit contenir une colonne `churn` avec 0 ou 1)
4. Lire les résultats et les explications affichées dans l'interface

## Exemple de structure de données attendue
| age | gender | monthly_spend | contract_type   | tenure | churn |
|-----|--------|---------------|----------------|--------|-------|
| 25  | M      | 30            | month-to-month | 12     | 1     |
| 45  | F      | 70            | one-year       | 36     | 0     |

## Auteur
Lionel FRITZ 