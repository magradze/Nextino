# extra_scripts/nextino_scripts/config_aggregator.py
"""
This module is responsible for finding and processing Nextino modules.
It scans the project's 'lib' directory, identifies valid Nextino modules
via a keyword in `library.json`, and aggregates their `config.json` files.
"""

import os
import json
import sys

# The keyword that identifies a library as a Nextino module.
MODULE_KEYWORD = "nextino-module"

def _is_nextino_module(lib_path):
    """
    Checks if a given library path contains a `library.json` with the
    Nextino module keyword.
    """
    lib_json_path = os.path.join(lib_path, "library.json")
    if not os.path.exists(lib_json_path):
        return False

    try:
        with open(lib_json_path, 'r', encoding='utf-8') as f:
            lib_data = json.load(f)
            keywords = lib_data.get("keywords", "")
            return MODULE_KEYWORD in keywords.lower()
    except Exception as e:
        print(f"Warning: Could not parse {lib_json_path}: {e}", file=sys.stderr)
        return False

def find_and_process_modules(project_lib_dir):
    """
    Scans the project library directory, finds all valid Nextino modules,
    and returns their aggregated configuration and metadata. It supports both
    single object and array of objects in `config.json` files.

    Args:
        project_lib_dir (str): The path to the project's 'lib' directory.

    Returns:
        dict: A dictionary containing lists of module configurations, headers, and class names.
    """
    all_module_configs = []
    unique_module_headers = set()
    unique_module_class_names = set()

    if not os.path.exists(project_lib_dir):
        return {
            "configs": [],
            "headers": [],
            "class_names": []
        }

    for lib_name in os.listdir(project_lib_dir):
        lib_path = os.path.join(project_lib_dir, lib_name)
        if not os.path.isdir(lib_path) or not _is_nextino_module(lib_path):
            continue

        print(f"  -> Found Nextino module: {lib_name}")

        # Process config.json
        config_path = os.path.join(lib_path, "config.json")
        if os.path.exists(config_path):
            try:
                with open(config_path, 'r', encoding='utf-8') as f:
                    config_data = json.load(f)
                    # Support both a single config object and an array of configs
                    if isinstance(config_data, list):
                        all_module_configs.extend(config_data)
                    else:
                        all_module_configs.append(config_data)
            except Exception as e:
                print(f"Warning: Could not parse {config_path}: {e}", file=sys.stderr)
        
        # Process header file to find the class name (we only need one per module type)
        src_dir = os.path.join(lib_path, "src")
        if os.path.exists(src_dir):
            for header_file in os.listdir(src_dir):
                if header_file.endswith(".h"):
                    class_name = header_file.replace(".h", "")
                    unique_module_headers.add(f'#include <{header_file}>')
                    unique_module_class_names.add(class_name)
                    # We assume one public header per module library
                    break
    
    return {
        "configs": all_module_configs,
        "headers": sorted(list(unique_module_headers)),
        "class_names": sorted(list(unique_module_class_names))
    }