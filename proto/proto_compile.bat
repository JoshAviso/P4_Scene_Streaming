:: If having issues with finding the vcpkg file, replace below with your full path to the vcpkg install folder
set vcpkg_path=%VCPKG_ROOT%

set protobuf_cmd="%vcpkg_path%\packages\protobuf_x64-windows\tools\protobuf\protoc"
set grpc_exe_dir="%vcpkg_path%\installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe"

set src="%cd%"
set proto_file="%cd%\SceneStreaming.proto"
set dest="%cd%\generated"

%protobuf_cmd% --proto_path=%src% --cpp_out=%dest% --grpc_out=%dest% --plugin=protoc-gen-grpc=%grpc_exe_dir% %proto_file%