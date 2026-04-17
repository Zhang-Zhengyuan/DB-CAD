from pydantic import Field
from pydantic_settings import BaseSettings, SettingsConfigDict


class Settings(BaseSettings):
    app_name: str = "CAD DB Store API"
    database_url: str = Field(default="sqlite:///./cad_store.db")

    model_config = SettingsConfigDict(env_prefix="CAD_DB_", env_file=".env", extra="ignore")


settings = Settings()
