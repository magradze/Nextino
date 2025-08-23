# extra_scripts/bootstrap.py
"""
Main entry point for the Nextino build process automation.

This script acts as a conductor, orchestrating the different parts of the build
process:
1. It aggregates configurations from all Nextino modules in the project.
2. It generates the `generated_config.h` header file containing the
   aggregated configuration and module registration code.
3. It configures the PlatformIO build environment to include the generated header.
"""

import os
from SCons.Script import Import

# Import our custom build modules
from nextino_scripts import config_aggregator
from nextino_scripts import code_generator

# Import the environment
Import("env")

def main(build_env):
    """The main function orchestrating the build process."""
    project_dir = build_env.get("PROJECT_DIR")
    project_include_dir = os.path.join(project_dir, "include")
    project_lib_dir = os.path.join(project_dir, "lib")
    
    print(f"--- [Nextino Bootstrap] Running for project: {project_dir} ---")

    # --- Step 1: Aggregate configurations from all found modules ---
    module_data = config_aggregator.find_and_process_modules(project_lib_dir)

    # --- Step 2: Generate the C++ header file content ---
    header_content = code_generator.generate_header_file(module_data)

    # --- Step 3: Write the generated content to the header file ---
    # Ensure the 'include' directory exists
    if not os.path.exists(project_include_dir):
        os.makedirs(project_include_dir)
    
    generated_header_path = os.path.join(project_include_dir, code_generator.GENERATED_HEADER_NAME)
    with open(generated_header_path, 'w', encoding='utf-8') as f:
        f.write(header_content)

    print(f"--- [Nextino Bootstrap] Finished: '{generated_header_path}' created. ---")

    # --- Step 4: Add the include path to the PlatformIO build environment ---
    build_env.Prepend(CPPPATH=[project_include_dir])
    print(f"--- [Nextino] Added '{project_include_dir}' to build environment CPPPATH. ---")


# --- Main execution ---
if __name__ == "__main__":
    # The 'env' variable is globally available from PlatformIO's SCons environment
    main(env)