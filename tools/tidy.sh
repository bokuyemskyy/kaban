clang-tidy \
  -p build \
  --extra-arg=-isysroot \
  --extra-arg=$(xcrun --show-sdk-path) \
  $(find ./app -name '*.cpp' ! -path './app/interface/gui/stb_image_impl.cpp')