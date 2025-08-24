# extra_scripts/bootstrap.py
# FINAL, SIMPLIFIED, AND ROBUST VERSION

import os
import sys
from SCons.Script import Import

# Import the build environment from SCons. This MUST be at the top level.
Import("env")

try:
    # --- The definitive, "dumb" way to set up the Python path ---
    # 1. Get the project's root directory. This is always reliable.
    project_dir = env.get("PROJECT_DIR")

    # 2. Construct the absolute path to our scripts directory based on the known structure.
    #    We know our scripts are always inside `lib/Nextino/extra_scripts`.
    scripts_dir = os.path.join(project_dir, "lib", "Nextino", "extra_scripts")

    # 3. Add this bulletproof path to Python's system path.
    if scripts_dir not in sys.path:
        sys.path.insert(0, scripts_dir)
    # --- END PATH INJECTION ---

    # Now, with the path correctly set, import our modules.
    from nextino_scripts import config_aggregator, code_generator, mqtt_generator

except Exception as e:
    print(f"FATAL ERROR: Could not set up Nextino build environment.", file=sys.stderr)
    print(f"--> Error: {e}", file=sys.stderr)
    sys.exit(1)


def main(build_env):
    """The main function orchestrating the build process."""
    # We already have project_dir, but we can get it again for clarity.
    project_dir = build_env.get("PROJECT_DIR")
    project_include_dir = os.path.join(project_dir, "include")
    project_lib_dir = os.path.join(project_dir, "lib")
    
    print(f"--- [Nextino Bootstrap] Running for project: {project_dir} ---")

    # The rest of the logic remains the same.
    module_data = config_aggregator.find_and_process_modules(project_lib_dir)
    header_content = code_generator.generate_header_file(module_data)

    if not os.path.exists(project_include_dir):
        os.makedirs(project_include_dir)
    
    generated_header_path = os.path.join(project_include_dir, code_generator.GENERATED_HEADER_NAME)
    with open(generated_header_path, 'w', encoding='utf-8') as f:
        f.write(header_content)

    print(f"--- [Nextino Bootstrap] Finished: '{generated_header_path}' created. ---")
    
    mqtt_header_content = mqtt_generator.generate_mqtt_header(module_data)
    if mqtt_header_content:
        generated_mqtt_header_path = os.path.join(project_include_dir, mqtt_generator.GENERATED_HEADER_NAME)
        with open(generated_mqtt_header_path, 'w', encoding='utf-8') as f:
            f.write(mqtt_header_content)
        print(f"--- [Nextino Bootstrap] Finished: '{generated_mqtt_header_path}' created. ---")

    build_env.Prepend(CPPPATH=[project_include_dir])
    print(f"--- [Nextino] Added '{project_include_dir}' to build environment CPPPATH. ---")


# --- Main execution triggered by SCons ---
try:
    main(env)
except Exception as e:
    print(f"FATAL: Unhandled error in Nextino bootstrap script: {e}", file=sys.stderr)
    sys.exit(1)