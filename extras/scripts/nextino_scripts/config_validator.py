# extras/scripts/nextino_scripts/config_validator.py
"""
This module performs validation of module instance configurations against
their declared schemas.
"""
import sys

def validate_configs(module_data):
    """
    Validates all module instance configurations against their schemas.

    Args:
        module_data (dict): The aggregated data from config_aggregator.

    Returns:
        bool: True if all configurations are valid, False otherwise.
    """
    all_configs = module_data.get("configs", [])
    schemas = module_data.get("schemas", {})
    all_valid = True

    print("--- [Nextino Bootstrap] Phase: Validating module configurations... ---")

    for instance_config in all_configs:
        module_type = instance_config.get("type")
        instance_name = instance_config.get("instance_name", "unnamed")
        
        # Check if a schema exists for this module type
        if module_type not in schemas:
            continue # No schema to validate against, so we assume it's valid.

        schema = schemas[module_type]
        config_to_validate = instance_config.get("config", {})

        print(f"  -> Validating '{instance_name}' ({module_type})...")

        for key, rules in schema.items():
            is_required = rules.get("required", False)
            expected_type = rules.get("type")
            
            # Check for required keys
            if is_required and key not in config_to_validate:
                print(f"ERROR: Missing required key '{key}' for instance '{instance_name}' of type '{module_type}'.", file=sys.stderr)
                all_valid = False
                continue

            if key in config_to_validate:
                actual_value = config_to_validate[key]
                actual_type_str = type(actual_value).__name__
                
                # Simple type check (can be expanded later)
                if expected_type == "integer" and actual_type_str != "int":
                    print(f"ERROR: Invalid type for key '{key}' in instance '{instance_name}'. Expected '{expected_type}', got '{actual_type_str}'.", file=sys.stderr)
                    all_valid = False
                elif expected_type == "string" and actual_type_str != "str":
                    print(f"ERROR: Invalid type for key '{key}' in instance '{instance_name}'. Expected '{expected_type}', got '{actual_type_str}'.", file=sys.stderr)
                    all_valid = False
                elif expected_type == "object" and actual_type_str != "dict":
                    print(f"ERROR: Invalid type for key '{key}' in instance '{instance_name}'. Expected '{expected_type}', got '{actual_type_str}'.", file=sys.stderr)
                    all_valid = False

    return all_valid