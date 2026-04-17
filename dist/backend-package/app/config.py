from pydantic import Field
from pydantic_settings import BaseSettings, SettingsConfigDict


class Settings(BaseSettings):
    app_name: str = "CAD DB Store API"
    storage_backend: str = Field(default="sqlite")
    database_url: str = Field(default="sqlite:///./cad_store.db")
    neo4j_uri: str = Field(default="")
    neo4j_user: str = Field(default="neo4j")
    neo4j_password: str = Field(default="")
    neo4j_database: str = Field(default="neo4j")

    model_config = SettingsConfigDict(env_prefix="CAD_DB_", env_file=".env", extra="ignore")


settings = Settings()
