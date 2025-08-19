import os
import json
import sys
from SCons.Script import Import

Import("env")

# --- SCRIPT CONFIGURATION ---
GENERATED_HEADER_NAME = "generated_config.h"
MODULE_KEYWORD = "nextino-module"
# --- END CONFIGURATION ---

def build_nextino_config(env):
    """
    This function scans for modules and generates the config header.
    """
    project_dir = env.get("PROJECT_DIR")
    print(f"--- [Nextino Bootstrap] Running for project: {project_dir} ---")

    project_lib_dir = os.path.join(project_dir, "lib")
    project_include_dir = os.path.join(project_dir, "include")

    module_configs = []
    module_headers = []
    module_registrations = []

    if os.path.exists(project_lib_dir):
        for lib_name in os.listdir(project_lib_dir):
            lib_path = os.path.join(project_lib_dir, lib_name)
            if not os.path.isdir(lib_path):
                continue

            lib_json_path = os.path.join(lib_path, "library.json")
            is_nextino_module = False
            if os.path.exists(lib_json_path):
                try:
                    with open(lib_json_path, 'r', encoding='utf-8') as f:
                        lib_data = json.load(f)
                        keywords = lib_data.get("keywords", "")
                        if MODULE_KEYWORD in keywords.lower():
                            is_nextino_module = True
                except Exception as e:
                    print(f"Warning: Could not parse {lib_json_path}: {e}", file=sys.stderr)

            if not is_nextino_module:
                continue

            print(f"  -> Found Nextino module: {lib_name}")

            config_path = os.path.join(lib_path, "config.json")
            if os.path.exists(config_path):
                with open(config_path, 'r', encoding='utf-8') as f:
                    module_configs.append(json.load(f))

            src_dir = os.path.join(lib_path, "src")
            if os.path.exists(src_dir):
                for header in os.listdir(src_dir):
                    if header.endswith(".h"):
                        module_class_name = header.replace(".h", "")
                        module_headers.append(f'#include <{header}>')
                        module_registrations.append(f'    NextinoFactory().registerModule("{module_class_name}", {module_class_name}::create);')
                        break

    final_config = {"modules": module_configs}
    final_json_string = json.dumps(final_config, indent=2)

    if not os.path.exists(project_include_dir):
        os.makedirs(project_include_dir)

    header_content = f"""
#pragma once
#include <Nextino.h>
{chr(10).join(module_headers)}
const char* projectConfigJson = R"json(
{final_json_string}
)json";
void registerAllModuleTypes() {{
{chr(10).join(module_registrations)}
}}
"""
    generated_header_path = os.path.join(project_include_dir, GENERATED_HEADER_NAME)
    with open(generated_header_path, 'w') as f:
        f.write(header_content)

    print(f"--- [Nextino Bootstrap] Finished: '{generated_header_path}' created. ---")
    
    # --- მთავარი ცვლილება ---
    # ვეუბნებით PlatformIO-ს, რომ build გარემოს დაამატოს ახალი საძიებო გზა
    env.Prepend(CPPPATH=[project_include_dir])
    print(f"--- [Nextino] Added '{project_include_dir}' to build environment CPPPATH. ---")

# --- მთავარი ლოგიკა ---
# ჩვენ აღარ ვიყენებთ AddPreAction-ს. სკრიპტი პირდაპირ ეშვება.
build_nextino_config(env)