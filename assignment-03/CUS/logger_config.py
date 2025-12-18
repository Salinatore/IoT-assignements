import logging

import colorlog


def setup_logging():
    # Create a colored formatter
    formatter = colorlog.ColoredFormatter(
        "%(log_color)s%(levelname)s%(reset)s: [%(name)s] %(message)s [%(asctime)s]",
        datefmt="%Y-%m-%d %H:%M:%S",
        log_colors={
            "DEBUG": "cyan",
            "INFO": "green",
            "WARNING": "yellow",
            "ERROR": "red",
            "CRITICAL": "red,bg_white",
        },
    )

    # Get the root logger
    handler = logging.StreamHandler()
    handler.setFormatter(formatter)

    # Configure root logger
    root_logger = logging.getLogger()
    root_logger.setLevel(logging.INFO)
    root_logger.handlers.clear()  # Remove default handlers
    root_logger.addHandler(handler)
