# Ensures package points to .so file when importing

# Rename package resources to tachyon.so on startup
def _bootstrap():
    global _so, _bootstrap
    from pathlib import Path

    _so = Path(__file__).parent / "time_series_tools.so"
    del _bootstrap
    import_from(__name__, _so)


def import_from(name, filepath, file=None):
    import importlib.util, sys

    spec = importlib.util.spec_from_file_location(name, filepath)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return sys.modules[name]


# Run rename for proper import
_bootstrap()
