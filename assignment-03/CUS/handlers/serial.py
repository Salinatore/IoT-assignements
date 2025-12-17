import asyncio
import logging

from connections.serial import SerialManager
from model.model import Mode, State

logger = logging.getLogger(__name__)


class SerialHandler:
    """Handles all communication to and from serial"""

    # Message protocol constants
    _CUS_TO_WCS_STATUS_AUTOMATIC = "cus->wcs-st-automatic"
    _CUS_TO_WCS_STATUS_REMOTE_MANUAL = "cus->wcs-st-remote-manual"
    _CUS_TO_WCS_STATUS_UNCONNECTED = "cus->wcs-st-unconnected"
    _CUS_TO_WCS_OPENING_PERCENTAGE_PREFIX = "cus->wcs-op-"
    _WCS_TO_CUS_STATUS_AUTOMATIC = "wcs->cus-st-automatic"
    _WCS_TO_CUS_STATUS_LOCAL_MANUAL = "wcs->cus-st-local-manual"
    _WCS_TO_CUS_OPENING_PERCENTAGE_PREFIX = "wcs->cus-op-"
    _EXPECTED_SENDER_PREFIX = "wcs->"

    # Validation constants
    _MIN_PERCENTAGE = 0
    _MAX_PERCENTAGE = 90

    def __init__(self, connection_manager: SerialManager, state: State):
        self._connection_manager = connection_manager
        self._state = state
        self._mode_to_message = {
            Mode.AUTOMATIC: self._CUS_TO_WCS_STATUS_AUTOMATIC,
            Mode.REMOTE_MANUAL: self._CUS_TO_WCS_STATUS_REMOTE_MANUAL,
            Mode.UNCONNECTED: self._CUS_TO_WCS_STATUS_UNCONNECTED,
        }

    def send_opening_percentage_to_serial(self, percentage: int) -> None:
        """Send opening percentage command to serial device.

        Args:
            percentage: Opening percentage (0-90)

        Raises:
            ValueError: If percentage is outside valid range
        """
        if not self._MIN_PERCENTAGE <= percentage <= self._MAX_PERCENTAGE:
            raise ValueError(
                f"Percentage must be between {self._MIN_PERCENTAGE} and "
                f"{self._MAX_PERCENTAGE}. Got: {percentage}"
            )
        asyncio.create_task(self._process_send_op_update_async(percentage))

    def send_mode_update_to_serial(self, mode: Mode) -> None:
        """Send mode update command to serial device.

        Args:
            mode: Target operating mode (AUTOMATIC, REMOTE_MANUAL or UNCONNETED)

        Raises:
            ValueError: If mode is not valid
        """
        if mode not in self._mode_to_message:
            raise ValueError(
                f"Invalid mode. Expected one of {list(self._mode_to_message.keys())}, "
                f"got: {mode}"
            )
        asyncio.create_task(self._process_send_mode_update_async(mode))

    def handle_message_from_serial(self, msg: str) -> None:
        """Handle incoming message from serial device.

        Args:
            msg: Raw message string from serial
        """
        asyncio.create_task(self._process_incoming_message_async(msg))

    async def _process_send_op_update_async(self, percentage: int):
        """Send opening percentage update to serial device."""
        msg = f"{self._CUS_TO_WCS_OPENING_PERCENTAGE_PREFIX}{percentage}"
        await self._connection_manager.send(msg)

    async def _process_send_mode_update_async(self, mode: Mode):
        """Send mode update to serial device."""
        await self._connection_manager.send(self._mode_to_message[mode])

    async def _process_incoming_message_async(self, msg: str):
        """Process incoming message from serial device."""
        if not msg.startswith(self._EXPECTED_SENDER_PREFIX):
            logger.error(
                f"Invalid message sender. Expected 'wcs->' prefix. Message: [{msg}]"
            )
            return

        match msg:
            case self._WCS_TO_CUS_STATUS_AUTOMATIC:
                self._state.set_mode(Mode.AUTOMATIC)
            case self._WCS_TO_CUS_STATUS_LOCAL_MANUAL:
                self._state.set_mode(Mode.LOCAL_MANUAL)

            case _ if msg.startswith(self._WCS_TO_CUS_OPENING_PERCENTAGE_PREFIX):
                self._handle_percentage_message(msg)

            case _:
                logger.error(f"Unknown message type from serial. Message: [{msg}]")

    def _handle_percentage_message(self, msg: str):
        """Extract and process percentage from message."""
        percentage_str = msg.removeprefix(self._WCS_TO_CUS_OPENING_PERCENTAGE_PREFIX)
        try:
            percentage = int(percentage_str)
            self._state.set_opening_percentage(percentage)
        except ValueError:
            logger.error(f"Invalid percentage value. Message: [{msg}]")
