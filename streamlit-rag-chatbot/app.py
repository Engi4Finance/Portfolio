import os
import nest_asyncio
import streamlit as st
from llama_index.llms.openai import OpenAI
from llama_index.core import VectorStoreIndex
from llama_cloud_services import LlamaParse
from llama_index.core.node_parser import MarkdownElementNodeParser
from llama_index.embeddings.openai import OpenAIEmbedding
import pickle
from pathlib import Path
import hashlib
from httpx import Timeout
import logging
from dotenv import load_dotenv

# Charger les variables d'environnement
load_dotenv()

# Logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

nest_asyncio.apply()

# Récupérer les clés API
OPENAI_API_KEY = os.environ.get("OPENAI_API_KEY")
LLAMAPARSE_API_KEY = os.environ.get("LLAMAPARSE_API_KEY")
if not OPENAI_API_KEY or not LLAMAPARSE_API_KEY:
    st.error("Les clés OPENAI_API_KEY et LLAMAPARSE_API_KEY doivent être définies dans le fichier .env")
    st.stop()
os.environ["OPENAI_API_KEY"] = OPENAI_API_KEY

parser = LlamaParse(
    api_key=LLAMAPARSE_API_KEY,
    result_type="markdown",
    timeout=Timeout(30.0)
)
llm_gpt4o_mini = OpenAI(
    model="gpt-4o-mini",
    timeout=30.0
)
embed_model = OpenAIEmbedding(
    model="text-embedding-ada-002",
    timeout=30.0
)

st.set_page_config(page_title="RAG Chatbot")
st.title("🤖 RAG Chatbot (Streamlit)")

file_path = st.text_input("Chemin du fichier à indexer", "Completed_data.xlsx")
cache_dir = Path("cache")
cache_dir.mkdir(exist_ok=True)

def get_file_hash(file_path):
    with open(file_path, 'rb') as f:
        return hashlib.md5(f.read()).hexdigest()

@st.cache_resource
def load_index_and_objects(file_path):
    try:
        cache_file = cache_dir / "index_cache.pkl"
        hash_file = cache_dir / "file_hash.txt"
        current_hash = get_file_hash(file_path)
        if cache_file.exists() and hash_file.exists():
            with open(hash_file, 'r') as f:
                cached_hash = f.read()
            if current_hash == cached_hash:
                with open(cache_file, 'rb') as f:
                    return pickle.load(f)
        with st.spinner("📄 Chargement du document..."):
            documents = parser.load_data(file_path)
        if documents:
            with st.spinner("🔄 Traitement des données..."):
                node_parser = MarkdownElementNodeParser(
                    llm=llm_gpt4o_mini,
                    num_workers=2,
                    timeout=30.0
                )
                all_nodes = []
                batch_size = 10
                for i in range(0, min(50, len(documents)), batch_size):
                    batch = documents[i:i+batch_size]
                    with st.spinner(f"🔄 Traitement du lot {i//batch_size + 1}..."):
                        nodes = node_parser.get_nodes_from_documents(batch)
                        all_nodes.extend(nodes)
                base_nodes, objects = node_parser.get_nodes_and_objects(all_nodes)
                index = VectorStoreIndex(
                    nodes=base_nodes + objects,
                    llm=llm_gpt4o_mini,
                    embed_model=embed_model
                )
                with open(cache_file, 'wb') as f:
                    pickle.dump((index, documents), f)
                with open(hash_file, 'w') as f:
                    f.write(current_hash)
                return index, documents
        return None, None
    except Exception as e:
        logger.error(f"Error loading index: {str(e)}")
        st.error(f"❌ Erreur lors du chargement: {str(e)}")
        return None, None

@st.cache_data(ttl=3600)
def get_cached_response(prompt: str) -> str:
    return None

with st.spinner("📄 Initialisation..."):
    index, documents = load_index_and_objects(file_path)

if index:
    st.success("✅ Document prêt à l'emploi.")
    with st.expander("🔍 Aperçu du fichier markdown extrait :"):
        st.text(documents[0].get_content()[:100000])
    if "chat_history" not in st.session_state:
        st.session_state.chat_history = []
    query_engine = index.as_query_engine(
        similarity_top_k=5,
        llm=llm_gpt4o_mini,
        embed_model=embed_model,
        timeout=30.0
    )
    st.markdown("### 💬 Chat avec l'IA sur les données")
    for msg in st.session_state.chat_history:
        with st.chat_message(msg["role"]):
            st.markdown(msg["content"])
    prompt = st.chat_input("Pose ta question ici...")
    if prompt:
        st.session_state.chat_history.append({"role": "user", "content": prompt})
        with st.chat_message("user"):
            st.markdown(prompt)
        with st.spinner("✈️ L'IA réfléchit..."):
            cached_response = get_cached_response(prompt)
            if cached_response is None:
                try:
                    response = query_engine.query(prompt)
                    answer = response.response
                    st.session_state[f"response_{hash(prompt)}"] = answer
                except Exception as e:
                    logger.error(f"Error processing query: {str(e)}")
                    answer = f"❌ Erreur lors du traitement de la requête: {str(e)}"
            else:
                answer = cached_response
        st.session_state.chat_history.append({"role": "assistant", "content": answer})
        with st.chat_message("assistant"):
            st.markdown(answer)
else:
    st.error("❌ Erreur de chargement ou document non valide.") 