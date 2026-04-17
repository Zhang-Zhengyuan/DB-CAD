from pydantic import Field, field_validator
from pydantic_settings import BaseSettings, SettingsConfigDict


class Settings(BaseSettings):
    app_name: str = "CAD DB Store API"
    storage_backend: str = Field(default="neo4j")
    neo4j_uri: str = Field(default="")
    neo4j_user: str = Field(default="neo4j")
    neo4j_password: str = Field(default="")
    neo4j_database: str = Field(default="neo4j")
    storage_bridge_url: str = Field(default="http://127.0.0.1:8100")
    storage_bridge_timeout_seconds: float = Field(default=15.0)
    api_password: str = Field(default="")

    @field_validator("storage_backend")
    @classmethod
    def validate_storage_backend(cls, value: str) -> str:
        if value.lower() != "neo4j":
            raise ValueError("Only neo4j storage backend is supported")
        return "neo4j"

    model_config = SettingsConfigDict(env_prefix="CAD_DB_", env_file=".env", extra="ignore")


settings = Settings()
