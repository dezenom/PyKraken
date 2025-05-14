from typing import TYPE_CHECKING, Sequence
if TYPE_CHECKING:
    from pykraken import Event

def get() -> Sequence["Event"]:
    """Get all window user events"""
    ...