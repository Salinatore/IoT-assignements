from time import time
from fastapi import APIRouter, Query

from model.model import Mode, State, state

# Router for testing endpoints that manipulate system state
# NOTE: These endpoints use GET methods for easy manual testing (can be triggered
# directly from a browser).
router = APIRouter(prefix="/test", tags=["testing"])


@router.get("/", response_model=State)
async def root() -> State:
    """
    [TEST ONLY] Get the current system state.

    Returns:
        State: The current state object containing mode and water level information.
    """
    return state


@router.get("/remote_manual", response_model=State)
async def change_state_to_remote_manual() -> State:
    """
    [TEST ONLY] Switch the system to remote manual mode.

    Sets the operating mode to REMOTE_MANUAL, allowing manual control
    of the system remotely.

    Returns:
        State: The updated state object after mode change.
    """
    state.set_mode(Mode.REMOTE_MANUAL)
    return state


@router.get("/automatic", response_model=State)
async def change_state_to_automatic() -> State:
    """
    [TEST ONLY] Switch the system to automatic mode.

    Sets the operating mode to AUTOMATIC, enabling autonomous system operation.

    Returns:
        State: The updated state object after mode change.
    """
    state.set_mode(Mode.AUTOMATIC)
    return state


@router.get("/increase_water_level", response_model=State)
async def increase_water_level(amount: int = Query(default=1)) -> State:
    """
    [TEST ONLY] Increase the water level by a specified amount.

    Increments the current water level and updates the timestamp.

    Args:
        amount: The amount to increase the water level by (default: 1).
    Returns:
        State: The updated state object after water level increase.
    """
    state.set_level(state._water_level + amount, time())
    return state

@router.get("/decrease_water_level", response_model=State)
async def decrease_water_level(amount: int = Query(default=1)) -> State:
    """
    [TEST ONLY] Decrease the water level by a specified amount.

    Decrements the current water level and updates the timestamp.

    Args:
        amount: The amount to decrease the water level by (default: 1).
    Returns:
        State: The updated state object after water level decrease.
    """
    state.set_level(state._water_level - amount, time())
    return state
