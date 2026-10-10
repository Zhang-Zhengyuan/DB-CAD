"""Shared API authentication dependency; preserves optional-password behavior."""

from fastapi import Header, HTTPException, status
from .config import settings


def verify_api_password(x_api_password: str | None = Header(default=None)) -> None:
    if settings.api_password and x_api_password != settings.api_password:
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED, detail="Invalid API password"
        )
