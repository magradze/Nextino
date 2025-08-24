# extras/scripts/nextino_scripts/config_aggregator.py
"""
This module is responsible for finding and processing Nextino modules.
It scans the project's 'lib' directory, identifies valid Nextino modules
via a keyword in `library.json`, and aggregates their configurations,
schemas, and other metadata.
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
    Scans libs, finds Nextino modules, and returns aggregated configs and metadata.
    It now also extracts `mqtt_interface` and `schema.json` data.
    """
    all_module_configs = []
    unique_module_headers = set()
    unique_module_class_names = set()
    mqtt_interfaces = []
    schemas = {}

    if not os.path.exists(project_lib_dir):
        return {
            "configs": [],
            "headers": [],
            "class_names": [],
            "mqtt_interfaces": [],
            "schemas": {}
        }

    for lib_name in os.listdir(project_lib_dir):
        lib_path = os.path.join(project_lib_dir, lib_name)
        if not os.path.isdir(lib_path) or not _is_nextino_module(lib_path):
            continue

        print(f"  -> Found Nextino module: {lib_name}")

        # Step 1: Find the header and class name. This is needed for the schema key.
        class_name = ""
        src_dir = os.path.join(lib_path, "src")
        if os.path.exists(src_dir):
            for header_file in os.listdir(src_dir):
                if header_file.endswith(".h"):
                    class_name = header_file.replace(".h", "")
                    unique_module_headers.add(f'#include <{header_file}>')
                    unique_module_class_names.add(class_name)
                    break
        
        # If no header/class was found, we can't process this module further.
        if not class_name:
            continue
            
        # Step 2: Process config.json
        config_path = os.path.join(lib_path, "config.json")
        if os.path.exists(config_path):
            try:
                with open(config_path, 'r', encoding='utf-8') as f:
                    config_data = json.load(f)
                    
                    if not isinstance(config_data, list):
                        config_data = [config_data]

                    all_module_configs.extend(config_data)

                    # Extract MQTT interface from each instance
                    for instance_config in config_data:
                        instance_name = instance_config.get("instance_name")
                        module_type = instance_config.get("type")
                        mqtt_interface = instance_config.get("mqtt_interface")

                        if instance_name and module_type and mqtt_interface:
                            mqtt_interfaces.append({
                                "instance_name": instance_name,
                                "module_type": module_type,
                                "interface": mqtt_interface
                            })
            except Exception as e:
                print(f"Warning: Could not parse {config_path}: {e}", file=sys.stderr)
                
        # Step 3: Process schema.json, using the class_name as the key.
        schema_path = os.path.join(lib_path, "schema.json")
        if os.path.exists(schema_path):
            try:
                with open(schema_path, 'r', encoding='utf-8') as f:
                    schemas[class_name] = json.load(f)
                    print(f"    - Found schema for {class_name}")
            except Exception as e:
                print(f"Warning: Could not parse {schema_path}: {e}", file=sys.stderr)
    
    return {
        "configs": all_module_configs,
        "headers": sorted(list(unique_module_headers)),
        "class_names": sorted(list(unique_module_class_names)),
        "mqtt_interfaces": mqtt_interfaces,
        "schemas": schemas
    }