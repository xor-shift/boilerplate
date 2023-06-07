SHADERC=../../../../cmake-build-debug/thirdparty/bgfx.cmake/cmake/bgfx/shaderc

# platform profile name
COMBO_GLSL=(linux 440 glsl)
COMBO_VULKAN=(linux spirv vulkan)
COMBO_ESSL_WGPU=(asm.js 300_es essl)
COMBO_ESSL=(android 300_es essl)
COMBO_DX9=(windows s_3_0 d3d9)
COMBO_DX11=(windows s_5_0 d3d11)
COMBO_METAL=(ios metal metal)

COMBOS=(${COMBO_GLSL[@]} ${COMBO_VULKAN[@]} ${COMBO_ESSL_WGPU[@]} ${COMBO_METAL[@]})

OUT_PATH=../../../../run/shaders/
IN_PATH=.

IN_SHADERS=(imgui)


for shader_name in ${IN_SHADERS[@]}; do
  for i in $(seq 2 3 ${#COMBOS[@]}); do
    PLATFORM=${COMBOS[$((i-2))]}
    PROFILE=${COMBOS[$((i-1))]}
    NAME=${COMBOS[$((i))]}

    echo "compiling $shader_name for the triple ($PLATFORM $PROFILE $NAME)"

    $SHADERC -i ../../../../thirdparty/bgfx.cmake/bgfx/src --type vertex --platform "$PLATFORM" --profile "$PROFILE" -f "$IN_PATH/$shader_name.vs.sc" -o "$OUT_PATH/$shader_name.$NAME.vs.bin"
    $SHADERC -i ../../../../thirdparty/bgfx.cmake/bgfx/src --type fragment --platform "$PLATFORM" --profile "$PROFILE" -f "$IN_PATH/$shader_name.fs.sc" -o "$OUT_PATH/$shader_name.$NAME.fs.bin"
  done
done
