from fastapi import APIRouter

from model.model import Mode, State, state

# Router for testing endpoints that manipulate system state
router = APIRouter(prefix="/test", tags=["testing"])


@router.get("/")
async def root() -> State:
    """
    Get the current system state.

    Returns:
        State: The current state object containing mode and water level information.
    """
    return state


@router.get("/remote_manual")
async def change_state_to_remote_manual() -> State:
    """
    Switch the system to remote manual mode.

    Sets the operating mode to REMOTE_MANUAL, allowing manual control
    of the system remotely.

    Returns:
        State: The updated state object after mode change.
    """
    state.set_mode(Mode.REMOTE_MANUAL)
    return state


@router.get("/automatic")
async def change_state_to_automatic() -> State:
    """
    Switch the system to automatic mode.

    Sets the operating mode to AUTOMATIC, enabling autonomous system operation.

    Returns:
        State: The updated state object after mode change.
    """
    state.set_mode(Mode.AUTOMATIC)
    return state


@router.get("/increase_water_level")
async def increase_water_level() -> State:
    """
    Increment the water level by 1.

    Increases the current water level value by one unit for testing purposes.

    Returns:
        State: The updated state object with the new water level.
    """
    state.set_level(state._water_level + 1)
    return state


@router.get("/decrease_water_level")
async def decrease_water_level() -> State:
    """
    Decrement the water level by 1.

    Decreases the current water level value by one unit for testing purposes.

    Returns:
        State: The updated state object with the new water level.
    """
    state.set_level(state._water_level - 1)
    return state
