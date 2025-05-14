# Streamlit RAG Chatbot

Ce projet propose une interface Streamlit pour interroger un document (Excel ou markdown) via un chatbot RAG (Retrieval Augmented Generation) utilisant OpenAI et LlamaParse.

## Fonctionnalités

- Extraction et indexation de documents via LlamaParse
- Recherche sémantique et génération de réponses avec OpenAI
- Interface chat conviviale
- Mise en cache intelligente

## Installation

1. Clonez ce dépôt :

   ```bash
   git clone https://github.com/Engi4Finance/Portfolio.git
   cd Portfolio/streamlit-rag-chatbot
   ```

2. Installez les dépendances :

   ```bash
   pip install -r requirements.txt
   ```

3. Copiez `.env.example` en `.env` et renseignez vos clés API :

   ```bash
   cp .env.example .env
   ```

4. Placez votre fichier de données (ex : `Completed_data.xlsx`) dans le dossier.

## Lancement

```bash
streamlit run app.py
```

## Sécurité

**Ne partagez jamais vos vraies clés API.**  
Le fichier `.env` n'est pas versionné.

---

**Auteur** : Engi4Finance 