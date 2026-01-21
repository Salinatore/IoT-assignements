import asyncio
import logging

from connections.serial import SerialConnection
from model.model import Mode, State

logger = logging.getLogger(__name__)


class SerialHandler:
    """Handles all communication to and from serial"""

    # Message protocol constants
    _CUS_TO_WCS_STATUS_AUTOMATIC = "cus->wcs-st-automatic"
    _CUS_TO_WCS_STATUS_LOCAL_MANUAL = "cus->wcs-st-local-manual"
    _CUS_TO_WCS_STATUS_REMOTE_MANUAL = "cus->wcs-st-remote-manual"
    _CUS_TO_WCS_STATUS_UNCONNECTED = "cus->wcs-st-unconnected"
    _CUS_TO_WCS_OPENING_PERCENTAGE_PREFIX = "cus->wcs-op-"
    _WCS_TO_CUS_STATUS_AUTOMATIC = "wcs->cus-st-automatic"
    _WCS_TO_CUS_STATUS_LOCAL_MANUAL = "wcs->cus-st-local-manual"
    _WCS_TO_CUS_OPENING_PERCENTAGE_PREFIX = "wcs->cus-op-"
    _EXPECTED_LOG_MSG_PREFIX = "wcs->cus-lo:"
    _EXPECTED_SENDER_PREFIX = "wcs->"

    # Validation constants
    _MIN_PERCENTAGE = 0
    _MAX_PERCENTAGE = 100

    def __init__(self, connection_manager: SerialConnection, state: State):
        self._connection_manager: SerialConnection = connection_manager
        self._state: State = state
        self._mode_to_message = {
            Mode.AUTOMATIC: self._CUS_TO_WCS_STATUS_AUTOMATIC,
            Mode.LOCAL_MANUAL: self._CUS_TO_WCS_STATUS_LOCAL_MANUAL,
            Mode.REMOTE_MANUAL: self._CUS_TO_WCS_STATUS_REMOTE_MANUAL,
            Mode.UNCONNECTED: self._CUS_TO_WCS_STATUS_UNCONNECTED,
        }
        self._last_mode: Mode = self._state.mode
        self._last_opening_percentage: int = self._state.opening_percentage

    def send_state_update_to_serial(self) -> None:
        """Send state updates to serial interface when changes are detected.

        Creates a background task without blocking.
        """
        current_mode = self._state.mode
        current_opening_percentage = self._state.opening_percentage

        if self._last_mode != current_mode:
            if current_mode not in self._mode_to_message:
                raise ValueError(
                    f"Invalid mode. Expected one of {list(self._mode_to_message.keys())}, "
                    f"Current in state: {current_mode}"
                )
            asyncio.create_task(self._process_send_mode_update_async(current_mode))
            self._last_mode = current_mode

        if self._last_opening_percentage != current_opening_percentage:
            if self._state.mode == Mode.LOCAL_MANUAL:
                return

            if not (
                self._MIN_PERCENTAGE
                <= current_opening_percentage
                <= self._MAX_PERCENTAGE
            ):
                logger.info("Persentage: ")
                raise ValueError(
                    f"Invalid opening percetage. Expected between 0 and 100"
                    f"Current in state: {current_opening_percentage}"
                )
            asyncio.create_task(
                self._process_send_op_update_async(current_opening_percentage)
            )
            self._last_opening_percentage = current_opening_percentage

    def handle_message_from_serial(self, msg: str) -> None:
        """Handle incoming message from serial device.

        Creates a background task without blocking.

        Args:
            msg: Raw message string from serial
        """
        asyncio.create_task(self._process_message_from_serial(msg))

    async def _process_send_op_update_async(self, percentage: int) -> None:
        """Send opening percentage update to serial devicnote."""
        msg = f"{self._CUS_TO_WCS_OPENING_PERCENTAGE_PREFIX}{percentage}"
        await self._connection_manager.send(msg)

    async def _process_send_mode_update_async(self, mode: Mode) -> None:
        """Send mode update to serial device."""
        await self._connection_manager.send(self._mode_to_message[mode])

    async def _process_message_from_serial(self, msg: str) -> None:
        """Process incoming message from serial device."""
        if not msg.startswith(self._EXPECTED_SENDER_PREFIX):
            logger.error(
                f"Invalid message sender. Expected 'wcs->' prefix. Message: [{msg}]"
            )
            return

        if msg.startswith(self._EXPECTED_LOG_MSG_PREFIX):
            msg = msg.removeprefix(self._EXPECTED_LOG_MSG_PREFIX)
            logger.info(f"Log message from serial: [{msg}]")
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

    def _handle_percentage_message(self, msg: str) -> None:
        """Extract and process percentage from message."""
        percentage_str = msg.removeprefix(self._WCS_TO_CUS_OPENING_PERCENTAGE_PREFIX)
        percentage = None
        try:
            percentage = int(percentage_str)
            self._state.set_opening_percentage(percentage)
        except ValueError:
            logger.error(f"Invalid percentage value. Message: [{msg}]")
