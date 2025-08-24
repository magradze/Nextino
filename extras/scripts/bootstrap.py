# extras/scripts/bootstrap.py

import os
import sys
from SCons.Script import Import

# Import the build environment from SCons. This MUST be at the top level.
Import("env")

try:
    # --- The definitive, robust way to set up the Python path ---
    # 1. Get the project's root directory from the SCons environment.
    project_dir = env.get("PROJECT_DIR")

    # 2. Construct the absolute path to our scripts directory.
    # This assumes Nextino is a library within the project's 'lib' directory.
    # We use this known, relative path to build a reliable absolute path.
    scripts_dir = os.path.join(project_dir, "lib", "Nextino", "extras", "scripts")

    # 3. Add this bulletproof path to Python's system path.
    if scripts_dir not in sys.path:
        sys.path.insert(0, scripts_dir)
    # --- END PATH INJECTION ---

    # Now, with the path correctly set, import our modules.
    from nextino_scripts import config_aggregator, code_generator, mqtt_generator, config_validator

except Exception as e:
    print(f"FATAL ERROR: Could not set up Nextino build environment.", file=sys.stderr)
    print(f"--> Error: {e}", file=sys.stderr)
    sys.exit(1)


def main(build_env):
    """The main function orchestrating the build process."""
    project_dir = build_env.get("PROJECT_DIR")
    project_include_dir = os.path.join(project_dir, "include")
    project_lib_dir = os.path.join(project_dir, "lib")
    
    print(f"--- [Nextino Bootstrap] Running for project: {project_dir} ---")

    # Step 1: Aggregate all module data (configs, schemas, headers, etc.)
    module_data = config_aggregator.find_and_process_modules(project_lib_dir)

    # Step 2: Validate the aggregated configurations against their schemas.
    # This is a critical step that happens *before* code generation. If it fails, we stop.
    if not config_validator.validate_configs(module_data):
        print("FATAL ERROR: Configuration validation failed. Halting build.", file=sys.stderr)
        sys.exit(1)

    # Step 3: Generate the main C++ header file (`generated_config.h`).
    header_content = code_generator.generate_header_file(module_data)

    if not os.path.exists(project_include_dir):
        os.makedirs(project_include_dir)
    
    generated_header_path = os.path.join(project_include_dir, code_generator.GENERATED_HEADER_NAME)
    with open(generated_header_path, 'w', encoding='utf-8') as f:
        f.write(header_content)

    print(f"--- [Nextino Bootstrap] Finished: '{generated_header_path}' created. ---")
    
    # Step 4: Generate the MQTT header file (`generated_mqtt.h`) if there is MQTT data.
    mqtt_header_content = mqtt_generator.generate_mqtt_header(module_data)
    if mqtt_header_content:
        generated_mqtt_header_path = os.path.join(project_include_dir, mqtt_generator.GENERATED_HEADER_NAME)
        with open(generated_mqtt_header_path, 'w', encoding='utf-8') as f:
            f.write(mqtt_header_content)
        print(f"--- [Nextino Bootstrap] Finished: '{generated_mqtt_header_path}' created. ---")

    # Step 5: Add the project's 'include' directory to the build path for the compiler.
    build_env.Prepend(CPPPATH=[project_include_dir])
    print(f"--- [Nextino] Added '{project_include_dir}' to build environment CPPPATH. ---")


# --- Main execution triggered by SCons ---
try:
    main(env)
except Exception as e:
    print(f"FATAL: Unhandled error in Nextino bootstrap script: {e}", file=sys.stderr)
    sys.exit(1)