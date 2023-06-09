SHADERC="shaderc.sh"

# platform profile name
COMBO_GLSL=(linux 440 glsl)
COMBO_VULKAN=(linux spirv vulkan)
COMBO_ESSL_WGPU=(asm.js 300_es essl)
#COMBO_ESSL=(android 300_es essl)
#COMBO_DX9=(windows s_3_0 d3d9)
#COMBO_DX11=(windows s_5_0 d3d11)
COMBO_METAL=(ios metal metal)

COMBOS=(${COMBO_GLSL[@]} ${COMBO_VULKAN[@]} ${COMBO_ESSL_WGPU[@]} ${COMBO_METAL[@]})


function fail() {
  printf "building shaders failed: %s\n" "$1"
  exit 1
}

function build() {
  SHADERC=cmake-build-debug/thirdparty/bgfx.cmake/cmake/bgfx/shaderc
  IN_FILE=""
  SHADER_TYPE=""
  SHADER_VARYING=""
  SHADER_BIN_OUT=""
  SHADER_HDR_OUT=""
  INCLUDE_DIRS=("./thirdparty/bgfx.cmake/bgfx/src/")

  function help() {
    echo "shaderc.sh"
    echo "  -h: prints help"
    echo "  -v : vertex shader"
    echo "  -f : fragment shader"
    echo "  -c : compute shader"
    echo "  -i : input file"
    echo "  -o : header output directory"
    echo "  -b : binary output directory"

    exit 0
  }

  local OPTIND
  while getopts "hs:i:b:o:d:vfci:" opt; do
    case "$opt" in
      h)
        help
        ;;
      v)
        SHADER_TYPE="vertex"
        ;;
      f)
        SHADER_TYPE="fragment"
        ;;
      c)
        SHADER_TYPE="compute"
        ;;
      i)
        IN_FILE="$OPTARG"
        ;;
      o)
        SHADER_HDR_OUT="$OPTARG"
        ;;
      b)
        SHADER_BIN_OUT="$OPTARG"
        ;;
      d)
        SHADER_VARYING="$OPTARG"
        ;;
      i)
        INCLUDE_DIRS+=("$OPTARG")
        ;;
    esac
  done

  echo "SHADERC = $SHADERC"
  echo "IN_FILE = $IN_FILE"
  echo "SHADER_TYPE = $SHADER_TYPE"
  echo "SHADER_VARYING = $SHADER_VARYING"
  echo "SHADER_BIN_OUT = $SHADER_BIN_OUT"
  echo "SHADER_HDR_OUT = $SHADER_HDR_OUT"

  # platform profile name
  COMBO_GLSL=(linux 440 glsl)
  COMBO_VULKAN=(linux spirv vulkan)
  COMBO_ESSL_WGPU=(asm.js 300_es essl)
  #COMBO_ESSL=(android 300_es essl)
  #COMBO_DX9=(windows s_3_0 d3d9)
  #COMBO_DX11=(windows s_5_0 d3d11)
  COMBO_METAL=(ios metal metal)

  COMBOS=(${COMBO_GLSL[@]} ${COMBO_VULKAN[@]} ${COMBO_ESSL_WGPU[@]} ${COMBO_METAL[@]})

  [[ -d "$SHADER_BIN_OUT" ]] || fail "shader binary output directory \"$SHADER_BIN_OUT\" does not exist"
  [[ -d "$SHADER_HDR_OUT" ]] || fail "shader binary output directory \"$SHADER_HDR_OUT\" does not exist"

  SHADER_NAME=$(basename $IN_FILE)

  for j in $(seq 2 3 ${#COMBOS[@]}); do
    local PLATFORM=${COMBOS[$((j-2))]}
    local PROFILE=${COMBOS[$((j-1))]}
    local TRIPLE_NAME=${COMBOS[$((j))]}

    echo "compiling $IN_FILE for the triple ($PLATFORM $PROFILE $TRIPLE_NAME)"

    $SHADERC ${INCLUDE_DIRS[@]/#/-i } \
         --type $SHADER_TYPE \
         --platform "$PLATFORM" \
         --profile "$PROFILE" \
         -f "$IN_FILE" \
         -o "$SHADER_BIN_OUT/$SHADER_NAME.$TRIPLE_NAME.bin" \

    xxd --include -n "${SHADER_NAME}_${TRIPLE_NAME}" "$SHADER_BIN_OUT/$SHADER_NAME.$TRIPLE_NAME.bin" > "$SHADER_HDR_OUT/$SHADER_NAME.$TRIPLE_NAME.h"

  done
}

build -i src/boilerplate/shaders/imgui/imgui.fs -f -d src/boilerplate/shaders/imgui/varying.def.sc -b run/shaders/ -o inc/boilerplate/detail/generated_shaders/
build -i src/boilerplate/shaders/imgui/imgui.vs -v -d src/boilerplate/shaders/imgui/varying.def.sc -b run/shaders/ -o inc/boilerplate/detail/generated_shaders/
