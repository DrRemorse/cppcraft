##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=fbcraft
ConfigurationName      :=Debug
WorkspacePath          := "C:\Projects\cppcraft"
ProjectPath            := "C:\Projects\cppcraft"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=fwsGonzo
Date                   :=01.09.2013
CodeLitePath           :="C:\CodeLite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="fbcraft.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=windres
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)inc 
IncludePCH             := 
RcIncludePath          := $(IncludeSwitch). 
Libs                   := $(LibrarySwitch)winmm $(LibrarySwitch)bass $(LibrarySwitch)glfw $(LibrarySwitch)opengl32 
ArLibs                 :=  "winmm" "bass" "glfw" "opengl32" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)lib 

##
## Common variables
## AR, CXX, CC, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -std=gnu++11 -g -pthread -Wall -Wno-write-strings $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)


##
## User defined environment variables
##
CodeLiteDir:=C:\CodeLite
UNIT_TEST_PP_SRC_DIR:=C:\UnitTest++-1.3
Objects0=$(IntermediateDirectory)/source_atmosphere$(ObjectSuffix) $(IntermediateDirectory)/source_biome$(ObjectSuffix) $(IntermediateDirectory)/source_blockmodels$(ObjectSuffix) $(IntermediateDirectory)/source_blockmodels_crosses$(ObjectSuffix) $(IntermediateDirectory)/source_blockmodels_cubes$(ObjectSuffix) $(IntermediateDirectory)/source_blocks$(ObjectSuffix) $(IntermediateDirectory)/source_chat$(ObjectSuffix) $(IntermediateDirectory)/source_chunks$(ObjectSuffix) $(IntermediateDirectory)/source_columns$(ObjectSuffix) $(IntermediateDirectory)/source_compilers$(ObjectSuffix) \
	$(IntermediateDirectory)/source_compressor$(ObjectSuffix) $(IntermediateDirectory)/source_drawq$(ObjectSuffix) $(IntermediateDirectory)/source_flatlands$(ObjectSuffix) $(IntermediateDirectory)/source_generator$(ObjectSuffix) $(IntermediateDirectory)/source_items$(ObjectSuffix) $(IntermediateDirectory)/source_lighting$(ObjectSuffix) $(IntermediateDirectory)/source_lighttable$(ObjectSuffix) $(IntermediateDirectory)/source_main$(ObjectSuffix) $(IntermediateDirectory)/source_minimap$(ObjectSuffix) $(IntermediateDirectory)/source_player$(ObjectSuffix) \
	$(IntermediateDirectory)/source_precompiler$(ObjectSuffix) $(IntermediateDirectory)/source_precompq$(ObjectSuffix) $(IntermediateDirectory)/source_precomp_thread$(ObjectSuffix) $(IntermediateDirectory)/source_precomp_thread_process$(ObjectSuffix) $(IntermediateDirectory)/source_precomp_vemitter$(ObjectSuffix) $(IntermediateDirectory)/source_rendergrid$(ObjectSuffix) $(IntermediateDirectory)/source_renderman$(ObjectSuffix) $(IntermediateDirectory)/source_seamless$(ObjectSuffix) $(IntermediateDirectory)/source_sector$(ObjectSuffix) $(IntermediateDirectory)/source_sectors$(ObjectSuffix) \
	$(IntermediateDirectory)/source_shaderman$(ObjectSuffix) $(IntermediateDirectory)/source_soundman$(ObjectSuffix) $(IntermediateDirectory)/source_spiders$(ObjectSuffix) $(IntermediateDirectory)/source_sun$(ObjectSuffix) $(IntermediateDirectory)/source_textureman$(ObjectSuffix) $(IntermediateDirectory)/source_threading$(ObjectSuffix) $(IntermediateDirectory)/source_tiles$(ObjectSuffix) $(IntermediateDirectory)/source_torchlight$(ObjectSuffix) $(IntermediateDirectory)/source_voxelmodels$(ObjectSuffix) $(IntermediateDirectory)/source_world$(ObjectSuffix) \
	$(IntermediateDirectory)/source_worldbuilder$(ObjectSuffix) $(IntermediateDirectory)/source_worldmanager$(ObjectSuffix) $(IntermediateDirectory)/source_render_scene$(ObjectSuffix) $(IntermediateDirectory)/source_render_sky$(ObjectSuffix) $(IntermediateDirectory)/source_input$(ObjectSuffix) $(IntermediateDirectory)/source_render_sky_clouds$(ObjectSuffix) $(IntermediateDirectory)/source_player_inputs$(ObjectSuffix) $(IntermediateDirectory)/source_precomp_vcrosses$(ObjectSuffix) $(IntermediateDirectory)/source_precomp_thread_ao$(ObjectSuffix) $(IntermediateDirectory)/source_render_fsflare$(ObjectSuffix) \
	$(IntermediateDirectory)/source_render_fs$(ObjectSuffix) $(IntermediateDirectory)/source_torchlight_calc$(ObjectSuffix) $(IntermediateDirectory)/source_torchlight_gatherer$(ObjectSuffix) $(IntermediateDirectory)/source_render_gui$(ObjectSuffix) $(IntermediateDirectory)/source_particles$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/source_particles_render$(ObjectSuffix) $(IntermediateDirectory)/source_player_translate$(ObjectSuffix) $(IntermediateDirectory)/source_render_scene_camera$(ObjectSuffix) $(IntermediateDirectory)/source_player_actions$(ObjectSuffix) $(IntermediateDirectory)/source_blockmodels_leafs$(ObjectSuffix) \
	$(IntermediateDirectory)/source_render_scene_terrain$(ObjectSuffix) $(IntermediateDirectory)/source_spiders_modify$(ObjectSuffix) $(IntermediateDirectory)/source_player_logic$(ObjectSuffix) $(IntermediateDirectory)/source_player_physics$(ObjectSuffix) $(IntermediateDirectory)/source_render_gui_crosshair$(ObjectSuffix) $(IntermediateDirectory)/source_textureman_generated$(ObjectSuffix) $(IntermediateDirectory)/source_spiders_world$(ObjectSuffix) $(IntermediateDirectory)/source_blockmodels_ladders$(ObjectSuffix) $(IntermediateDirectory)/source_blockmodels_lanterns$(ObjectSuffix) $(IntermediateDirectory)/source_blockmodels_poles$(ObjectSuffix) \
	$(IntermediateDirectory)/source_precomp_vladders$(ObjectSuffix) $(IntermediateDirectory)/source_precomp_vlantern$(ObjectSuffix) $(IntermediateDirectory)/source_precomp_vpoles$(ObjectSuffix) $(IntermediateDirectory)/source_precomp_vsloped$(ObjectSuffix) $(IntermediateDirectory)/source_render_gui_compass$(ObjectSuffix) $(IntermediateDirectory)/source_precomp_vstairs$(ObjectSuffix) $(IntermediateDirectory)/source_blockmodels_stairs$(ObjectSuffix) $(IntermediateDirectory)/source_precomp_vfences$(ObjectSuffix) $(IntermediateDirectory)/source_blockmodels_fences$(ObjectSuffix) $(IntermediateDirectory)/source_precomp_vdoors$(ObjectSuffix) \
	$(IntermediateDirectory)/source_render_gui_playerhand$(ObjectSuffix) $(IntermediateDirectory)/source_render_gui_quickbar$(ObjectSuffix) $(IntermediateDirectory)/source_render_player_selection$(ObjectSuffix) $(IntermediateDirectory)/source_menu$(ObjectSuffix) $(IntermediateDirectory)/source_player_actions_inputs$(ObjectSuffix) $(IntermediateDirectory)/source_player_actions_handlers$(ObjectSuffix) $(IntermediateDirectory)/source_render_fsblur$(ObjectSuffix) $(IntermediateDirectory)/source_camera$(ObjectSuffix) $(IntermediateDirectory)/source_render_gui_inventory$(ObjectSuffix) $(IntermediateDirectory)/source_worldmanager_init$(ObjectSuffix) \
	$(IntermediateDirectory)/res_cppcraft.rc$(ObjectSuffix) $(IntermediateDirectory)/library_config$(ObjectSuffix) $(IntermediateDirectory)/library_log$(ObjectSuffix) $(IntermediateDirectory)/tests_gltest$(ObjectSuffix) $(IntermediateDirectory)/tests_socktest$(ObjectSuffix) $(IntermediateDirectory)/tests_testmain$(ObjectSuffix) $(IntermediateDirectory)/tests_threadtest$(ObjectSuffix) $(IntermediateDirectory)/tests_timetest$(ObjectSuffix) $(IntermediateDirectory)/bitmap_bitmap$(ObjectSuffix) $(IntermediateDirectory)/bitmap_colortools$(ObjectSuffix) \
	$(IntermediateDirectory)/bitmap_lodepng$(ObjectSuffix) $(IntermediateDirectory)/compression_lzo$(ObjectSuffix) $(IntermediateDirectory)/math_baseconv$(ObjectSuffix) 

Objects2=$(IntermediateDirectory)/math_frustum$(ObjectSuffix) $(IntermediateDirectory)/math_matrix$(ObjectSuffix) $(IntermediateDirectory)/math_quaternion$(ObjectSuffix) $(IntermediateDirectory)/math_vector$(ObjectSuffix) $(IntermediateDirectory)/math_voronoi$(ObjectSuffix) $(IntermediateDirectory)/math_vec3$(ObjectSuffix) $(IntermediateDirectory)/math_vec4$(ObjectSuffix) \
	$(IntermediateDirectory)/network_hostaddr$(ObjectSuffix) $(IntermediateDirectory)/network_socket$(ObjectSuffix) $(IntermediateDirectory)/network_tcpsocket$(ObjectSuffix) $(IntermediateDirectory)/noise_cosnoise$(ObjectSuffix) $(IntermediateDirectory)/noise_gabor$(ObjectSuffix) $(IntermediateDirectory)/noise_multifractal$(ObjectSuffix) $(IntermediateDirectory)/noise_noise1234$(ObjectSuffix) $(IntermediateDirectory)/noise_sdnoise1234$(ObjectSuffix) $(IntermediateDirectory)/noise_simplex1234$(ObjectSuffix) $(IntermediateDirectory)/noise_srdnoise23$(ObjectSuffix) \
	$(IntermediateDirectory)/noise_torusnoise$(ObjectSuffix) $(IntermediateDirectory)/noise_turbulence$(ObjectSuffix) $(IntermediateDirectory)/noise_worley$(ObjectSuffix) $(IntermediateDirectory)/opengl_oglfont$(ObjectSuffix) $(IntermediateDirectory)/opengl_opengl$(ObjectSuffix) $(IntermediateDirectory)/opengl_openglext$(ObjectSuffix) $(IntermediateDirectory)/opengl_shader$(ObjectSuffix) $(IntermediateDirectory)/opengl_texture$(ObjectSuffix) $(IntermediateDirectory)/opengl_window$(ObjectSuffix) $(IntermediateDirectory)/opengl_fbo$(ObjectSuffix) \
	$(IntermediateDirectory)/opengl_vao$(ObjectSuffix) $(IntermediateDirectory)/sound_sound$(ObjectSuffix) $(IntermediateDirectory)/sound_stream_channel$(ObjectSuffix) $(IntermediateDirectory)/sound_stream$(ObjectSuffix) $(IntermediateDirectory)/threading_thread_controller$(ObjectSuffix) $(IntermediateDirectory)/threading_thread_worker$(ObjectSuffix) $(IntermediateDirectory)/threading_thread_pool$(ObjectSuffix) $(IntermediateDirectory)/timing_highperf$(ObjectSuffix) $(IntermediateDirectory)/timing_timer$(ObjectSuffix) $(IntermediateDirectory)/voxels_voxelizer$(ObjectSuffix) \
	$(IntermediateDirectory)/voxels_voxelmodel$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) $(Objects2) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	@echo $(Objects2) >> $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/source_atmosphere$(ObjectSuffix): source/atmosphere.cpp $(IntermediateDirectory)/source_atmosphere$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/atmosphere.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_atmosphere$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_atmosphere$(DependSuffix): source/atmosphere.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_atmosphere$(ObjectSuffix) -MF$(IntermediateDirectory)/source_atmosphere$(DependSuffix) -MM "source/atmosphere.cpp"

$(IntermediateDirectory)/source_atmosphere$(PreprocessSuffix): source/atmosphere.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_atmosphere$(PreprocessSuffix) "source/atmosphere.cpp"

$(IntermediateDirectory)/source_biome$(ObjectSuffix): source/biome.cpp $(IntermediateDirectory)/source_biome$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/biome.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_biome$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_biome$(DependSuffix): source/biome.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_biome$(ObjectSuffix) -MF$(IntermediateDirectory)/source_biome$(DependSuffix) -MM "source/biome.cpp"

$(IntermediateDirectory)/source_biome$(PreprocessSuffix): source/biome.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_biome$(PreprocessSuffix) "source/biome.cpp"

$(IntermediateDirectory)/source_blockmodels$(ObjectSuffix): source/blockmodels.cpp $(IntermediateDirectory)/source_blockmodels$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/blockmodels.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_blockmodels$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_blockmodels$(DependSuffix): source/blockmodels.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_blockmodels$(ObjectSuffix) -MF$(IntermediateDirectory)/source_blockmodels$(DependSuffix) -MM "source/blockmodels.cpp"

$(IntermediateDirectory)/source_blockmodels$(PreprocessSuffix): source/blockmodels.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_blockmodels$(PreprocessSuffix) "source/blockmodels.cpp"

$(IntermediateDirectory)/source_blockmodels_crosses$(ObjectSuffix): source/blockmodels_crosses.cpp $(IntermediateDirectory)/source_blockmodels_crosses$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/blockmodels_crosses.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_blockmodels_crosses$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_blockmodels_crosses$(DependSuffix): source/blockmodels_crosses.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_blockmodels_crosses$(ObjectSuffix) -MF$(IntermediateDirectory)/source_blockmodels_crosses$(DependSuffix) -MM "source/blockmodels_crosses.cpp"

$(IntermediateDirectory)/source_blockmodels_crosses$(PreprocessSuffix): source/blockmodels_crosses.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_blockmodels_crosses$(PreprocessSuffix) "source/blockmodels_crosses.cpp"

$(IntermediateDirectory)/source_blockmodels_cubes$(ObjectSuffix): source/blockmodels_cubes.cpp $(IntermediateDirectory)/source_blockmodels_cubes$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/blockmodels_cubes.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_blockmodels_cubes$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_blockmodels_cubes$(DependSuffix): source/blockmodels_cubes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_blockmodels_cubes$(ObjectSuffix) -MF$(IntermediateDirectory)/source_blockmodels_cubes$(DependSuffix) -MM "source/blockmodels_cubes.cpp"

$(IntermediateDirectory)/source_blockmodels_cubes$(PreprocessSuffix): source/blockmodels_cubes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_blockmodels_cubes$(PreprocessSuffix) "source/blockmodels_cubes.cpp"

$(IntermediateDirectory)/source_blocks$(ObjectSuffix): source/blocks.cpp $(IntermediateDirectory)/source_blocks$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/blocks.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_blocks$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_blocks$(DependSuffix): source/blocks.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_blocks$(ObjectSuffix) -MF$(IntermediateDirectory)/source_blocks$(DependSuffix) -MM "source/blocks.cpp"

$(IntermediateDirectory)/source_blocks$(PreprocessSuffix): source/blocks.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_blocks$(PreprocessSuffix) "source/blocks.cpp"

$(IntermediateDirectory)/source_chat$(ObjectSuffix): source/chat.cpp $(IntermediateDirectory)/source_chat$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/chat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_chat$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_chat$(DependSuffix): source/chat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_chat$(ObjectSuffix) -MF$(IntermediateDirectory)/source_chat$(DependSuffix) -MM "source/chat.cpp"

$(IntermediateDirectory)/source_chat$(PreprocessSuffix): source/chat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_chat$(PreprocessSuffix) "source/chat.cpp"

$(IntermediateDirectory)/source_chunks$(ObjectSuffix): source/chunks.cpp $(IntermediateDirectory)/source_chunks$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/chunks.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_chunks$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_chunks$(DependSuffix): source/chunks.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_chunks$(ObjectSuffix) -MF$(IntermediateDirectory)/source_chunks$(DependSuffix) -MM "source/chunks.cpp"

$(IntermediateDirectory)/source_chunks$(PreprocessSuffix): source/chunks.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_chunks$(PreprocessSuffix) "source/chunks.cpp"

$(IntermediateDirectory)/source_columns$(ObjectSuffix): source/columns.cpp $(IntermediateDirectory)/source_columns$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/columns.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_columns$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_columns$(DependSuffix): source/columns.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_columns$(ObjectSuffix) -MF$(IntermediateDirectory)/source_columns$(DependSuffix) -MM "source/columns.cpp"

$(IntermediateDirectory)/source_columns$(PreprocessSuffix): source/columns.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_columns$(PreprocessSuffix) "source/columns.cpp"

$(IntermediateDirectory)/source_compilers$(ObjectSuffix): source/compilers.cpp $(IntermediateDirectory)/source_compilers$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/compilers.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_compilers$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_compilers$(DependSuffix): source/compilers.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_compilers$(ObjectSuffix) -MF$(IntermediateDirectory)/source_compilers$(DependSuffix) -MM "source/compilers.cpp"

$(IntermediateDirectory)/source_compilers$(PreprocessSuffix): source/compilers.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_compilers$(PreprocessSuffix) "source/compilers.cpp"

$(IntermediateDirectory)/source_compressor$(ObjectSuffix): source/compressor.cpp $(IntermediateDirectory)/source_compressor$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/compressor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_compressor$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_compressor$(DependSuffix): source/compressor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_compressor$(ObjectSuffix) -MF$(IntermediateDirectory)/source_compressor$(DependSuffix) -MM "source/compressor.cpp"

$(IntermediateDirectory)/source_compressor$(PreprocessSuffix): source/compressor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_compressor$(PreprocessSuffix) "source/compressor.cpp"

$(IntermediateDirectory)/source_drawq$(ObjectSuffix): source/drawq.cpp $(IntermediateDirectory)/source_drawq$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/drawq.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_drawq$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_drawq$(DependSuffix): source/drawq.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_drawq$(ObjectSuffix) -MF$(IntermediateDirectory)/source_drawq$(DependSuffix) -MM "source/drawq.cpp"

$(IntermediateDirectory)/source_drawq$(PreprocessSuffix): source/drawq.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_drawq$(PreprocessSuffix) "source/drawq.cpp"

$(IntermediateDirectory)/source_flatlands$(ObjectSuffix): source/flatlands.cpp $(IntermediateDirectory)/source_flatlands$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/flatlands.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_flatlands$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_flatlands$(DependSuffix): source/flatlands.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_flatlands$(ObjectSuffix) -MF$(IntermediateDirectory)/source_flatlands$(DependSuffix) -MM "source/flatlands.cpp"

$(IntermediateDirectory)/source_flatlands$(PreprocessSuffix): source/flatlands.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_flatlands$(PreprocessSuffix) "source/flatlands.cpp"

$(IntermediateDirectory)/source_generator$(ObjectSuffix): source/generator.cpp $(IntermediateDirectory)/source_generator$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/generator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_generator$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_generator$(DependSuffix): source/generator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_generator$(ObjectSuffix) -MF$(IntermediateDirectory)/source_generator$(DependSuffix) -MM "source/generator.cpp"

$(IntermediateDirectory)/source_generator$(PreprocessSuffix): source/generator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_generator$(PreprocessSuffix) "source/generator.cpp"

$(IntermediateDirectory)/source_items$(ObjectSuffix): source/items.cpp $(IntermediateDirectory)/source_items$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/items.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_items$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_items$(DependSuffix): source/items.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_items$(ObjectSuffix) -MF$(IntermediateDirectory)/source_items$(DependSuffix) -MM "source/items.cpp"

$(IntermediateDirectory)/source_items$(PreprocessSuffix): source/items.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_items$(PreprocessSuffix) "source/items.cpp"

$(IntermediateDirectory)/source_lighting$(ObjectSuffix): source/lighting.cpp $(IntermediateDirectory)/source_lighting$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/lighting.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_lighting$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_lighting$(DependSuffix): source/lighting.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_lighting$(ObjectSuffix) -MF$(IntermediateDirectory)/source_lighting$(DependSuffix) -MM "source/lighting.cpp"

$(IntermediateDirectory)/source_lighting$(PreprocessSuffix): source/lighting.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_lighting$(PreprocessSuffix) "source/lighting.cpp"

$(IntermediateDirectory)/source_lighttable$(ObjectSuffix): source/lighttable.cpp $(IntermediateDirectory)/source_lighttable$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/lighttable.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_lighttable$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_lighttable$(DependSuffix): source/lighttable.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_lighttable$(ObjectSuffix) -MF$(IntermediateDirectory)/source_lighttable$(DependSuffix) -MM "source/lighttable.cpp"

$(IntermediateDirectory)/source_lighttable$(PreprocessSuffix): source/lighttable.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_lighttable$(PreprocessSuffix) "source/lighttable.cpp"

$(IntermediateDirectory)/source_main$(ObjectSuffix): source/main.cpp $(IntermediateDirectory)/source_main$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_main$(DependSuffix): source/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_main$(ObjectSuffix) -MF$(IntermediateDirectory)/source_main$(DependSuffix) -MM "source/main.cpp"

$(IntermediateDirectory)/source_main$(PreprocessSuffix): source/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_main$(PreprocessSuffix) "source/main.cpp"

$(IntermediateDirectory)/source_minimap$(ObjectSuffix): source/minimap.cpp $(IntermediateDirectory)/source_minimap$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/minimap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_minimap$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_minimap$(DependSuffix): source/minimap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_minimap$(ObjectSuffix) -MF$(IntermediateDirectory)/source_minimap$(DependSuffix) -MM "source/minimap.cpp"

$(IntermediateDirectory)/source_minimap$(PreprocessSuffix): source/minimap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_minimap$(PreprocessSuffix) "source/minimap.cpp"

$(IntermediateDirectory)/source_player$(ObjectSuffix): source/player.cpp $(IntermediateDirectory)/source_player$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/player.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_player$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_player$(DependSuffix): source/player.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_player$(ObjectSuffix) -MF$(IntermediateDirectory)/source_player$(DependSuffix) -MM "source/player.cpp"

$(IntermediateDirectory)/source_player$(PreprocessSuffix): source/player.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_player$(PreprocessSuffix) "source/player.cpp"

$(IntermediateDirectory)/source_precompiler$(ObjectSuffix): source/precompiler.cpp $(IntermediateDirectory)/source_precompiler$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/precompiler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_precompiler$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_precompiler$(DependSuffix): source/precompiler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_precompiler$(ObjectSuffix) -MF$(IntermediateDirectory)/source_precompiler$(DependSuffix) -MM "source/precompiler.cpp"

$(IntermediateDirectory)/source_precompiler$(PreprocessSuffix): source/precompiler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_precompiler$(PreprocessSuffix) "source/precompiler.cpp"

$(IntermediateDirectory)/source_precompq$(ObjectSuffix): source/precompq.cpp $(IntermediateDirectory)/source_precompq$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/precompq.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_precompq$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_precompq$(DependSuffix): source/precompq.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_precompq$(ObjectSuffix) -MF$(IntermediateDirectory)/source_precompq$(DependSuffix) -MM "source/precompq.cpp"

$(IntermediateDirectory)/source_precompq$(PreprocessSuffix): source/precompq.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_precompq$(PreprocessSuffix) "source/precompq.cpp"

$(IntermediateDirectory)/source_precomp_thread$(ObjectSuffix): source/precomp_thread.cpp $(IntermediateDirectory)/source_precomp_thread$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/precomp_thread.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_precomp_thread$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_precomp_thread$(DependSuffix): source/precomp_thread.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_precomp_thread$(ObjectSuffix) -MF$(IntermediateDirectory)/source_precomp_thread$(DependSuffix) -MM "source/precomp_thread.cpp"

$(IntermediateDirectory)/source_precomp_thread$(PreprocessSuffix): source/precomp_thread.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_precomp_thread$(PreprocessSuffix) "source/precomp_thread.cpp"

$(IntermediateDirectory)/source_precomp_thread_process$(ObjectSuffix): source/precomp_thread_process.cpp $(IntermediateDirectory)/source_precomp_thread_process$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/precomp_thread_process.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_precomp_thread_process$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_precomp_thread_process$(DependSuffix): source/precomp_thread_process.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_precomp_thread_process$(ObjectSuffix) -MF$(IntermediateDirectory)/source_precomp_thread_process$(DependSuffix) -MM "source/precomp_thread_process.cpp"

$(IntermediateDirectory)/source_precomp_thread_process$(PreprocessSuffix): source/precomp_thread_process.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_precomp_thread_process$(PreprocessSuffix) "source/precomp_thread_process.cpp"

$(IntermediateDirectory)/source_precomp_vemitter$(ObjectSuffix): source/precomp_vemitter.cpp $(IntermediateDirectory)/source_precomp_vemitter$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/precomp_vemitter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_precomp_vemitter$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_precomp_vemitter$(DependSuffix): source/precomp_vemitter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_precomp_vemitter$(ObjectSuffix) -MF$(IntermediateDirectory)/source_precomp_vemitter$(DependSuffix) -MM "source/precomp_vemitter.cpp"

$(IntermediateDirectory)/source_precomp_vemitter$(PreprocessSuffix): source/precomp_vemitter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_precomp_vemitter$(PreprocessSuffix) "source/precomp_vemitter.cpp"

$(IntermediateDirectory)/source_rendergrid$(ObjectSuffix): source/rendergrid.cpp $(IntermediateDirectory)/source_rendergrid$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/rendergrid.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_rendergrid$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_rendergrid$(DependSuffix): source/rendergrid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_rendergrid$(ObjectSuffix) -MF$(IntermediateDirectory)/source_rendergrid$(DependSuffix) -MM "source/rendergrid.cpp"

$(IntermediateDirectory)/source_rendergrid$(PreprocessSuffix): source/rendergrid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_rendergrid$(PreprocessSuffix) "source/rendergrid.cpp"

$(IntermediateDirectory)/source_renderman$(ObjectSuffix): source/renderman.cpp $(IntermediateDirectory)/source_renderman$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/renderman.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_renderman$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_renderman$(DependSuffix): source/renderman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_renderman$(ObjectSuffix) -MF$(IntermediateDirectory)/source_renderman$(DependSuffix) -MM "source/renderman.cpp"

$(IntermediateDirectory)/source_renderman$(PreprocessSuffix): source/renderman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_renderman$(PreprocessSuffix) "source/renderman.cpp"

$(IntermediateDirectory)/source_seamless$(ObjectSuffix): source/seamless.cpp $(IntermediateDirectory)/source_seamless$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/seamless.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_seamless$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_seamless$(DependSuffix): source/seamless.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_seamless$(ObjectSuffix) -MF$(IntermediateDirectory)/source_seamless$(DependSuffix) -MM "source/seamless.cpp"

$(IntermediateDirectory)/source_seamless$(PreprocessSuffix): source/seamless.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_seamless$(PreprocessSuffix) "source/seamless.cpp"

$(IntermediateDirectory)/source_sector$(ObjectSuffix): source/sector.cpp $(IntermediateDirectory)/source_sector$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/sector.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_sector$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_sector$(DependSuffix): source/sector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_sector$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sector$(DependSuffix) -MM "source/sector.cpp"

$(IntermediateDirectory)/source_sector$(PreprocessSuffix): source/sector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_sector$(PreprocessSuffix) "source/sector.cpp"

$(IntermediateDirectory)/source_sectors$(ObjectSuffix): source/sectors.cpp $(IntermediateDirectory)/source_sectors$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/sectors.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_sectors$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_sectors$(DependSuffix): source/sectors.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_sectors$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sectors$(DependSuffix) -MM "source/sectors.cpp"

$(IntermediateDirectory)/source_sectors$(PreprocessSuffix): source/sectors.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_sectors$(PreprocessSuffix) "source/sectors.cpp"

$(IntermediateDirectory)/source_shaderman$(ObjectSuffix): source/shaderman.cpp $(IntermediateDirectory)/source_shaderman$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/shaderman.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_shaderman$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_shaderman$(DependSuffix): source/shaderman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_shaderman$(ObjectSuffix) -MF$(IntermediateDirectory)/source_shaderman$(DependSuffix) -MM "source/shaderman.cpp"

$(IntermediateDirectory)/source_shaderman$(PreprocessSuffix): source/shaderman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_shaderman$(PreprocessSuffix) "source/shaderman.cpp"

$(IntermediateDirectory)/source_soundman$(ObjectSuffix): source/soundman.cpp $(IntermediateDirectory)/source_soundman$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/soundman.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_soundman$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_soundman$(DependSuffix): source/soundman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_soundman$(ObjectSuffix) -MF$(IntermediateDirectory)/source_soundman$(DependSuffix) -MM "source/soundman.cpp"

$(IntermediateDirectory)/source_soundman$(PreprocessSuffix): source/soundman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_soundman$(PreprocessSuffix) "source/soundman.cpp"

$(IntermediateDirectory)/source_spiders$(ObjectSuffix): source/spiders.cpp $(IntermediateDirectory)/source_spiders$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/spiders.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_spiders$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_spiders$(DependSuffix): source/spiders.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_spiders$(ObjectSuffix) -MF$(IntermediateDirectory)/source_spiders$(DependSuffix) -MM "source/spiders.cpp"

$(IntermediateDirectory)/source_spiders$(PreprocessSuffix): source/spiders.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_spiders$(PreprocessSuffix) "source/spiders.cpp"

$(IntermediateDirectory)/source_sun$(ObjectSuffix): source/sun.cpp $(IntermediateDirectory)/source_sun$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/sun.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_sun$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_sun$(DependSuffix): source/sun.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_sun$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sun$(DependSuffix) -MM "source/sun.cpp"

$(IntermediateDirectory)/source_sun$(PreprocessSuffix): source/sun.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_sun$(PreprocessSuffix) "source/sun.cpp"

$(IntermediateDirectory)/source_textureman$(ObjectSuffix): source/textureman.cpp $(IntermediateDirectory)/source_textureman$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/textureman.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_textureman$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_textureman$(DependSuffix): source/textureman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_textureman$(ObjectSuffix) -MF$(IntermediateDirectory)/source_textureman$(DependSuffix) -MM "source/textureman.cpp"

$(IntermediateDirectory)/source_textureman$(PreprocessSuffix): source/textureman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_textureman$(PreprocessSuffix) "source/textureman.cpp"

$(IntermediateDirectory)/source_threading$(ObjectSuffix): source/threading.cpp $(IntermediateDirectory)/source_threading$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/threading.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_threading$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_threading$(DependSuffix): source/threading.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_threading$(ObjectSuffix) -MF$(IntermediateDirectory)/source_threading$(DependSuffix) -MM "source/threading.cpp"

$(IntermediateDirectory)/source_threading$(PreprocessSuffix): source/threading.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_threading$(PreprocessSuffix) "source/threading.cpp"

$(IntermediateDirectory)/source_tiles$(ObjectSuffix): source/tiles.cpp $(IntermediateDirectory)/source_tiles$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/tiles.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_tiles$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_tiles$(DependSuffix): source/tiles.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_tiles$(ObjectSuffix) -MF$(IntermediateDirectory)/source_tiles$(DependSuffix) -MM "source/tiles.cpp"

$(IntermediateDirectory)/source_tiles$(PreprocessSuffix): source/tiles.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_tiles$(PreprocessSuffix) "source/tiles.cpp"

$(IntermediateDirectory)/source_torchlight$(ObjectSuffix): source/torchlight.cpp $(IntermediateDirectory)/source_torchlight$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/torchlight.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_torchlight$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_torchlight$(DependSuffix): source/torchlight.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_torchlight$(ObjectSuffix) -MF$(IntermediateDirectory)/source_torchlight$(DependSuffix) -MM "source/torchlight.cpp"

$(IntermediateDirectory)/source_torchlight$(PreprocessSuffix): source/torchlight.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_torchlight$(PreprocessSuffix) "source/torchlight.cpp"

$(IntermediateDirectory)/source_voxelmodels$(ObjectSuffix): source/voxelmodels.cpp $(IntermediateDirectory)/source_voxelmodels$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/voxelmodels.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_voxelmodels$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_voxelmodels$(DependSuffix): source/voxelmodels.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_voxelmodels$(ObjectSuffix) -MF$(IntermediateDirectory)/source_voxelmodels$(DependSuffix) -MM "source/voxelmodels.cpp"

$(IntermediateDirectory)/source_voxelmodels$(PreprocessSuffix): source/voxelmodels.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_voxelmodels$(PreprocessSuffix) "source/voxelmodels.cpp"

$(IntermediateDirectory)/source_world$(ObjectSuffix): source/world.cpp $(IntermediateDirectory)/source_world$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/world.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_world$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_world$(DependSuffix): source/world.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_world$(ObjectSuffix) -MF$(IntermediateDirectory)/source_world$(DependSuffix) -MM "source/world.cpp"

$(IntermediateDirectory)/source_world$(PreprocessSuffix): source/world.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_world$(PreprocessSuffix) "source/world.cpp"

$(IntermediateDirectory)/source_worldbuilder$(ObjectSuffix): source/worldbuilder.cpp $(IntermediateDirectory)/source_worldbuilder$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/worldbuilder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_worldbuilder$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_worldbuilder$(DependSuffix): source/worldbuilder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_worldbuilder$(ObjectSuffix) -MF$(IntermediateDirectory)/source_worldbuilder$(DependSuffix) -MM "source/worldbuilder.cpp"

$(IntermediateDirectory)/source_worldbuilder$(PreprocessSuffix): source/worldbuilder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_worldbuilder$(PreprocessSuffix) "source/worldbuilder.cpp"

$(IntermediateDirectory)/source_worldmanager$(ObjectSuffix): source/worldmanager.cpp $(IntermediateDirectory)/source_worldmanager$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/worldmanager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_worldmanager$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_worldmanager$(DependSuffix): source/worldmanager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_worldmanager$(ObjectSuffix) -MF$(IntermediateDirectory)/source_worldmanager$(DependSuffix) -MM "source/worldmanager.cpp"

$(IntermediateDirectory)/source_worldmanager$(PreprocessSuffix): source/worldmanager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_worldmanager$(PreprocessSuffix) "source/worldmanager.cpp"

$(IntermediateDirectory)/source_render_scene$(ObjectSuffix): source/render_scene.cpp $(IntermediateDirectory)/source_render_scene$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/render_scene.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_render_scene$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_render_scene$(DependSuffix): source/render_scene.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_render_scene$(ObjectSuffix) -MF$(IntermediateDirectory)/source_render_scene$(DependSuffix) -MM "source/render_scene.cpp"

$(IntermediateDirectory)/source_render_scene$(PreprocessSuffix): source/render_scene.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_render_scene$(PreprocessSuffix) "source/render_scene.cpp"

$(IntermediateDirectory)/source_render_sky$(ObjectSuffix): source/render_sky.cpp $(IntermediateDirectory)/source_render_sky$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/render_sky.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_render_sky$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_render_sky$(DependSuffix): source/render_sky.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_render_sky$(ObjectSuffix) -MF$(IntermediateDirectory)/source_render_sky$(DependSuffix) -MM "source/render_sky.cpp"

$(IntermediateDirectory)/source_render_sky$(PreprocessSuffix): source/render_sky.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_render_sky$(PreprocessSuffix) "source/render_sky.cpp"

$(IntermediateDirectory)/source_input$(ObjectSuffix): source/input.cpp $(IntermediateDirectory)/source_input$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/input.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_input$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_input$(DependSuffix): source/input.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_input$(ObjectSuffix) -MF$(IntermediateDirectory)/source_input$(DependSuffix) -MM "source/input.cpp"

$(IntermediateDirectory)/source_input$(PreprocessSuffix): source/input.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_input$(PreprocessSuffix) "source/input.cpp"

$(IntermediateDirectory)/source_render_sky_clouds$(ObjectSuffix): source/render_sky_clouds.cpp $(IntermediateDirectory)/source_render_sky_clouds$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/render_sky_clouds.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_render_sky_clouds$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_render_sky_clouds$(DependSuffix): source/render_sky_clouds.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_render_sky_clouds$(ObjectSuffix) -MF$(IntermediateDirectory)/source_render_sky_clouds$(DependSuffix) -MM "source/render_sky_clouds.cpp"

$(IntermediateDirectory)/source_render_sky_clouds$(PreprocessSuffix): source/render_sky_clouds.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_render_sky_clouds$(PreprocessSuffix) "source/render_sky_clouds.cpp"

$(IntermediateDirectory)/source_player_inputs$(ObjectSuffix): source/player_inputs.cpp $(IntermediateDirectory)/source_player_inputs$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/player_inputs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_player_inputs$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_player_inputs$(DependSuffix): source/player_inputs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_player_inputs$(ObjectSuffix) -MF$(IntermediateDirectory)/source_player_inputs$(DependSuffix) -MM "source/player_inputs.cpp"

$(IntermediateDirectory)/source_player_inputs$(PreprocessSuffix): source/player_inputs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_player_inputs$(PreprocessSuffix) "source/player_inputs.cpp"

$(IntermediateDirectory)/source_precomp_vcrosses$(ObjectSuffix): source/precomp_vcrosses.cpp $(IntermediateDirectory)/source_precomp_vcrosses$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/precomp_vcrosses.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_precomp_vcrosses$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_precomp_vcrosses$(DependSuffix): source/precomp_vcrosses.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_precomp_vcrosses$(ObjectSuffix) -MF$(IntermediateDirectory)/source_precomp_vcrosses$(DependSuffix) -MM "source/precomp_vcrosses.cpp"

$(IntermediateDirectory)/source_precomp_vcrosses$(PreprocessSuffix): source/precomp_vcrosses.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_precomp_vcrosses$(PreprocessSuffix) "source/precomp_vcrosses.cpp"

$(IntermediateDirectory)/source_precomp_thread_ao$(ObjectSuffix): source/precomp_thread_ao.cpp $(IntermediateDirectory)/source_precomp_thread_ao$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/precomp_thread_ao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_precomp_thread_ao$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_precomp_thread_ao$(DependSuffix): source/precomp_thread_ao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_precomp_thread_ao$(ObjectSuffix) -MF$(IntermediateDirectory)/source_precomp_thread_ao$(DependSuffix) -MM "source/precomp_thread_ao.cpp"

$(IntermediateDirectory)/source_precomp_thread_ao$(PreprocessSuffix): source/precomp_thread_ao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_precomp_thread_ao$(PreprocessSuffix) "source/precomp_thread_ao.cpp"

$(IntermediateDirectory)/source_render_fsflare$(ObjectSuffix): source/render_fsflare.cpp $(IntermediateDirectory)/source_render_fsflare$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/render_fsflare.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_render_fsflare$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_render_fsflare$(DependSuffix): source/render_fsflare.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_render_fsflare$(ObjectSuffix) -MF$(IntermediateDirectory)/source_render_fsflare$(DependSuffix) -MM "source/render_fsflare.cpp"

$(IntermediateDirectory)/source_render_fsflare$(PreprocessSuffix): source/render_fsflare.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_render_fsflare$(PreprocessSuffix) "source/render_fsflare.cpp"

$(IntermediateDirectory)/source_render_fs$(ObjectSuffix): source/render_fs.cpp $(IntermediateDirectory)/source_render_fs$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/render_fs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_render_fs$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_render_fs$(DependSuffix): source/render_fs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_render_fs$(ObjectSuffix) -MF$(IntermediateDirectory)/source_render_fs$(DependSuffix) -MM "source/render_fs.cpp"

$(IntermediateDirectory)/source_render_fs$(PreprocessSuffix): source/render_fs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_render_fs$(PreprocessSuffix) "source/render_fs.cpp"

$(IntermediateDirectory)/source_torchlight_calc$(ObjectSuffix): source/torchlight_calc.cpp $(IntermediateDirectory)/source_torchlight_calc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/torchlight_calc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_torchlight_calc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_torchlight_calc$(DependSuffix): source/torchlight_calc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_torchlight_calc$(ObjectSuffix) -MF$(IntermediateDirectory)/source_torchlight_calc$(DependSuffix) -MM "source/torchlight_calc.cpp"

$(IntermediateDirectory)/source_torchlight_calc$(PreprocessSuffix): source/torchlight_calc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_torchlight_calc$(PreprocessSuffix) "source/torchlight_calc.cpp"

$(IntermediateDirectory)/source_torchlight_gatherer$(ObjectSuffix): source/torchlight_gatherer.cpp $(IntermediateDirectory)/source_torchlight_gatherer$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/torchlight_gatherer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_torchlight_gatherer$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_torchlight_gatherer$(DependSuffix): source/torchlight_gatherer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_torchlight_gatherer$(ObjectSuffix) -MF$(IntermediateDirectory)/source_torchlight_gatherer$(DependSuffix) -MM "source/torchlight_gatherer.cpp"

$(IntermediateDirectory)/source_torchlight_gatherer$(PreprocessSuffix): source/torchlight_gatherer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_torchlight_gatherer$(PreprocessSuffix) "source/torchlight_gatherer.cpp"

$(IntermediateDirectory)/source_render_gui$(ObjectSuffix): source/render_gui.cpp $(IntermediateDirectory)/source_render_gui$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/render_gui.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_render_gui$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_render_gui$(DependSuffix): source/render_gui.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_render_gui$(ObjectSuffix) -MF$(IntermediateDirectory)/source_render_gui$(DependSuffix) -MM "source/render_gui.cpp"

$(IntermediateDirectory)/source_render_gui$(PreprocessSuffix): source/render_gui.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_render_gui$(PreprocessSuffix) "source/render_gui.cpp"

$(IntermediateDirectory)/source_particles$(ObjectSuffix): source/particles.cpp $(IntermediateDirectory)/source_particles$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/particles.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_particles$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_particles$(DependSuffix): source/particles.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_particles$(ObjectSuffix) -MF$(IntermediateDirectory)/source_particles$(DependSuffix) -MM "source/particles.cpp"

$(IntermediateDirectory)/source_particles$(PreprocessSuffix): source/particles.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_particles$(PreprocessSuffix) "source/particles.cpp"

$(IntermediateDirectory)/source_particles_render$(ObjectSuffix): source/particles_render.cpp $(IntermediateDirectory)/source_particles_render$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/particles_render.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_particles_render$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_particles_render$(DependSuffix): source/particles_render.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_particles_render$(ObjectSuffix) -MF$(IntermediateDirectory)/source_particles_render$(DependSuffix) -MM "source/particles_render.cpp"

$(IntermediateDirectory)/source_particles_render$(PreprocessSuffix): source/particles_render.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_particles_render$(PreprocessSuffix) "source/particles_render.cpp"

$(IntermediateDirectory)/source_player_translate$(ObjectSuffix): source/player_translate.cpp $(IntermediateDirectory)/source_player_translate$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/player_translate.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_player_translate$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_player_translate$(DependSuffix): source/player_translate.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_player_translate$(ObjectSuffix) -MF$(IntermediateDirectory)/source_player_translate$(DependSuffix) -MM "source/player_translate.cpp"

$(IntermediateDirectory)/source_player_translate$(PreprocessSuffix): source/player_translate.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_player_translate$(PreprocessSuffix) "source/player_translate.cpp"

$(IntermediateDirectory)/source_render_scene_camera$(ObjectSuffix): source/render_scene_camera.cpp $(IntermediateDirectory)/source_render_scene_camera$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/render_scene_camera.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_render_scene_camera$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_render_scene_camera$(DependSuffix): source/render_scene_camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_render_scene_camera$(ObjectSuffix) -MF$(IntermediateDirectory)/source_render_scene_camera$(DependSuffix) -MM "source/render_scene_camera.cpp"

$(IntermediateDirectory)/source_render_scene_camera$(PreprocessSuffix): source/render_scene_camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_render_scene_camera$(PreprocessSuffix) "source/render_scene_camera.cpp"

$(IntermediateDirectory)/source_player_actions$(ObjectSuffix): source/player_actions.cpp $(IntermediateDirectory)/source_player_actions$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/player_actions.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_player_actions$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_player_actions$(DependSuffix): source/player_actions.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_player_actions$(ObjectSuffix) -MF$(IntermediateDirectory)/source_player_actions$(DependSuffix) -MM "source/player_actions.cpp"

$(IntermediateDirectory)/source_player_actions$(PreprocessSuffix): source/player_actions.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_player_actions$(PreprocessSuffix) "source/player_actions.cpp"

$(IntermediateDirectory)/source_blockmodels_leafs$(ObjectSuffix): source/blockmodels_leafs.cpp $(IntermediateDirectory)/source_blockmodels_leafs$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/blockmodels_leafs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_blockmodels_leafs$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_blockmodels_leafs$(DependSuffix): source/blockmodels_leafs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_blockmodels_leafs$(ObjectSuffix) -MF$(IntermediateDirectory)/source_blockmodels_leafs$(DependSuffix) -MM "source/blockmodels_leafs.cpp"

$(IntermediateDirectory)/source_blockmodels_leafs$(PreprocessSuffix): source/blockmodels_leafs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_blockmodels_leafs$(PreprocessSuffix) "source/blockmodels_leafs.cpp"

$(IntermediateDirectory)/source_render_scene_terrain$(ObjectSuffix): source/render_scene_terrain.cpp $(IntermediateDirectory)/source_render_scene_terrain$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/render_scene_terrain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_render_scene_terrain$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_render_scene_terrain$(DependSuffix): source/render_scene_terrain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_render_scene_terrain$(ObjectSuffix) -MF$(IntermediateDirectory)/source_render_scene_terrain$(DependSuffix) -MM "source/render_scene_terrain.cpp"

$(IntermediateDirectory)/source_render_scene_terrain$(PreprocessSuffix): source/render_scene_terrain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_render_scene_terrain$(PreprocessSuffix) "source/render_scene_terrain.cpp"

$(IntermediateDirectory)/source_spiders_modify$(ObjectSuffix): source/spiders_modify.cpp $(IntermediateDirectory)/source_spiders_modify$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/spiders_modify.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_spiders_modify$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_spiders_modify$(DependSuffix): source/spiders_modify.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_spiders_modify$(ObjectSuffix) -MF$(IntermediateDirectory)/source_spiders_modify$(DependSuffix) -MM "source/spiders_modify.cpp"

$(IntermediateDirectory)/source_spiders_modify$(PreprocessSuffix): source/spiders_modify.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_spiders_modify$(PreprocessSuffix) "source/spiders_modify.cpp"

$(IntermediateDirectory)/source_player_logic$(ObjectSuffix): source/player_logic.cpp $(IntermediateDirectory)/source_player_logic$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/player_logic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_player_logic$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_player_logic$(DependSuffix): source/player_logic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_player_logic$(ObjectSuffix) -MF$(IntermediateDirectory)/source_player_logic$(DependSuffix) -MM "source/player_logic.cpp"

$(IntermediateDirectory)/source_player_logic$(PreprocessSuffix): source/player_logic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_player_logic$(PreprocessSuffix) "source/player_logic.cpp"

$(IntermediateDirectory)/source_player_physics$(ObjectSuffix): source/player_physics.cpp $(IntermediateDirectory)/source_player_physics$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/player_physics.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_player_physics$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_player_physics$(DependSuffix): source/player_physics.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_player_physics$(ObjectSuffix) -MF$(IntermediateDirectory)/source_player_physics$(DependSuffix) -MM "source/player_physics.cpp"

$(IntermediateDirectory)/source_player_physics$(PreprocessSuffix): source/player_physics.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_player_physics$(PreprocessSuffix) "source/player_physics.cpp"

$(IntermediateDirectory)/source_render_gui_crosshair$(ObjectSuffix): source/render_gui_crosshair.cpp $(IntermediateDirectory)/source_render_gui_crosshair$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/render_gui_crosshair.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_render_gui_crosshair$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_render_gui_crosshair$(DependSuffix): source/render_gui_crosshair.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_render_gui_crosshair$(ObjectSuffix) -MF$(IntermediateDirectory)/source_render_gui_crosshair$(DependSuffix) -MM "source/render_gui_crosshair.cpp"

$(IntermediateDirectory)/source_render_gui_crosshair$(PreprocessSuffix): source/render_gui_crosshair.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_render_gui_crosshair$(PreprocessSuffix) "source/render_gui_crosshair.cpp"

$(IntermediateDirectory)/source_textureman_generated$(ObjectSuffix): source/textureman_generated.cpp $(IntermediateDirectory)/source_textureman_generated$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/textureman_generated.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_textureman_generated$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_textureman_generated$(DependSuffix): source/textureman_generated.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_textureman_generated$(ObjectSuffix) -MF$(IntermediateDirectory)/source_textureman_generated$(DependSuffix) -MM "source/textureman_generated.cpp"

$(IntermediateDirectory)/source_textureman_generated$(PreprocessSuffix): source/textureman_generated.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_textureman_generated$(PreprocessSuffix) "source/textureman_generated.cpp"

$(IntermediateDirectory)/source_spiders_world$(ObjectSuffix): source/spiders_world.cpp $(IntermediateDirectory)/source_spiders_world$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/spiders_world.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_spiders_world$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_spiders_world$(DependSuffix): source/spiders_world.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_spiders_world$(ObjectSuffix) -MF$(IntermediateDirectory)/source_spiders_world$(DependSuffix) -MM "source/spiders_world.cpp"

$(IntermediateDirectory)/source_spiders_world$(PreprocessSuffix): source/spiders_world.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_spiders_world$(PreprocessSuffix) "source/spiders_world.cpp"

$(IntermediateDirectory)/source_blockmodels_ladders$(ObjectSuffix): source/blockmodels_ladders.cpp $(IntermediateDirectory)/source_blockmodels_ladders$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/blockmodels_ladders.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_blockmodels_ladders$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_blockmodels_ladders$(DependSuffix): source/blockmodels_ladders.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_blockmodels_ladders$(ObjectSuffix) -MF$(IntermediateDirectory)/source_blockmodels_ladders$(DependSuffix) -MM "source/blockmodels_ladders.cpp"

$(IntermediateDirectory)/source_blockmodels_ladders$(PreprocessSuffix): source/blockmodels_ladders.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_blockmodels_ladders$(PreprocessSuffix) "source/blockmodels_ladders.cpp"

$(IntermediateDirectory)/source_blockmodels_lanterns$(ObjectSuffix): source/blockmodels_lanterns.cpp $(IntermediateDirectory)/source_blockmodels_lanterns$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/blockmodels_lanterns.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_blockmodels_lanterns$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_blockmodels_lanterns$(DependSuffix): source/blockmodels_lanterns.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_blockmodels_lanterns$(ObjectSuffix) -MF$(IntermediateDirectory)/source_blockmodels_lanterns$(DependSuffix) -MM "source/blockmodels_lanterns.cpp"

$(IntermediateDirectory)/source_blockmodels_lanterns$(PreprocessSuffix): source/blockmodels_lanterns.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_blockmodels_lanterns$(PreprocessSuffix) "source/blockmodels_lanterns.cpp"

$(IntermediateDirectory)/source_blockmodels_poles$(ObjectSuffix): source/blockmodels_poles.cpp $(IntermediateDirectory)/source_blockmodels_poles$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/blockmodels_poles.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_blockmodels_poles$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_blockmodels_poles$(DependSuffix): source/blockmodels_poles.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_blockmodels_poles$(ObjectSuffix) -MF$(IntermediateDirectory)/source_blockmodels_poles$(DependSuffix) -MM "source/blockmodels_poles.cpp"

$(IntermediateDirectory)/source_blockmodels_poles$(PreprocessSuffix): source/blockmodels_poles.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_blockmodels_poles$(PreprocessSuffix) "source/blockmodels_poles.cpp"

$(IntermediateDirectory)/source_precomp_vladders$(ObjectSuffix): source/precomp_vladders.cpp $(IntermediateDirectory)/source_precomp_vladders$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/precomp_vladders.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_precomp_vladders$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_precomp_vladders$(DependSuffix): source/precomp_vladders.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_precomp_vladders$(ObjectSuffix) -MF$(IntermediateDirectory)/source_precomp_vladders$(DependSuffix) -MM "source/precomp_vladders.cpp"

$(IntermediateDirectory)/source_precomp_vladders$(PreprocessSuffix): source/precomp_vladders.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_precomp_vladders$(PreprocessSuffix) "source/precomp_vladders.cpp"

$(IntermediateDirectory)/source_precomp_vlantern$(ObjectSuffix): source/precomp_vlantern.cpp $(IntermediateDirectory)/source_precomp_vlantern$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/precomp_vlantern.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_precomp_vlantern$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_precomp_vlantern$(DependSuffix): source/precomp_vlantern.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_precomp_vlantern$(ObjectSuffix) -MF$(IntermediateDirectory)/source_precomp_vlantern$(DependSuffix) -MM "source/precomp_vlantern.cpp"

$(IntermediateDirectory)/source_precomp_vlantern$(PreprocessSuffix): source/precomp_vlantern.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_precomp_vlantern$(PreprocessSuffix) "source/precomp_vlantern.cpp"

$(IntermediateDirectory)/source_precomp_vpoles$(ObjectSuffix): source/precomp_vpoles.cpp $(IntermediateDirectory)/source_precomp_vpoles$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/precomp_vpoles.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_precomp_vpoles$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_precomp_vpoles$(DependSuffix): source/precomp_vpoles.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_precomp_vpoles$(ObjectSuffix) -MF$(IntermediateDirectory)/source_precomp_vpoles$(DependSuffix) -MM "source/precomp_vpoles.cpp"

$(IntermediateDirectory)/source_precomp_vpoles$(PreprocessSuffix): source/precomp_vpoles.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_precomp_vpoles$(PreprocessSuffix) "source/precomp_vpoles.cpp"

$(IntermediateDirectory)/source_precomp_vsloped$(ObjectSuffix): source/precomp_vsloped.cpp $(IntermediateDirectory)/source_precomp_vsloped$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/precomp_vsloped.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_precomp_vsloped$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_precomp_vsloped$(DependSuffix): source/precomp_vsloped.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_precomp_vsloped$(ObjectSuffix) -MF$(IntermediateDirectory)/source_precomp_vsloped$(DependSuffix) -MM "source/precomp_vsloped.cpp"

$(IntermediateDirectory)/source_precomp_vsloped$(PreprocessSuffix): source/precomp_vsloped.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_precomp_vsloped$(PreprocessSuffix) "source/precomp_vsloped.cpp"

$(IntermediateDirectory)/source_render_gui_compass$(ObjectSuffix): source/render_gui_compass.cpp $(IntermediateDirectory)/source_render_gui_compass$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/render_gui_compass.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_render_gui_compass$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_render_gui_compass$(DependSuffix): source/render_gui_compass.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_render_gui_compass$(ObjectSuffix) -MF$(IntermediateDirectory)/source_render_gui_compass$(DependSuffix) -MM "source/render_gui_compass.cpp"

$(IntermediateDirectory)/source_render_gui_compass$(PreprocessSuffix): source/render_gui_compass.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_render_gui_compass$(PreprocessSuffix) "source/render_gui_compass.cpp"

$(IntermediateDirectory)/source_precomp_vstairs$(ObjectSuffix): source/precomp_vstairs.cpp $(IntermediateDirectory)/source_precomp_vstairs$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/precomp_vstairs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_precomp_vstairs$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_precomp_vstairs$(DependSuffix): source/precomp_vstairs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_precomp_vstairs$(ObjectSuffix) -MF$(IntermediateDirectory)/source_precomp_vstairs$(DependSuffix) -MM "source/precomp_vstairs.cpp"

$(IntermediateDirectory)/source_precomp_vstairs$(PreprocessSuffix): source/precomp_vstairs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_precomp_vstairs$(PreprocessSuffix) "source/precomp_vstairs.cpp"

$(IntermediateDirectory)/source_blockmodels_stairs$(ObjectSuffix): source/blockmodels_stairs.cpp $(IntermediateDirectory)/source_blockmodels_stairs$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/blockmodels_stairs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_blockmodels_stairs$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_blockmodels_stairs$(DependSuffix): source/blockmodels_stairs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_blockmodels_stairs$(ObjectSuffix) -MF$(IntermediateDirectory)/source_blockmodels_stairs$(DependSuffix) -MM "source/blockmodels_stairs.cpp"

$(IntermediateDirectory)/source_blockmodels_stairs$(PreprocessSuffix): source/blockmodels_stairs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_blockmodels_stairs$(PreprocessSuffix) "source/blockmodels_stairs.cpp"

$(IntermediateDirectory)/source_precomp_vfences$(ObjectSuffix): source/precomp_vfences.cpp $(IntermediateDirectory)/source_precomp_vfences$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/precomp_vfences.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_precomp_vfences$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_precomp_vfences$(DependSuffix): source/precomp_vfences.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_precomp_vfences$(ObjectSuffix) -MF$(IntermediateDirectory)/source_precomp_vfences$(DependSuffix) -MM "source/precomp_vfences.cpp"

$(IntermediateDirectory)/source_precomp_vfences$(PreprocessSuffix): source/precomp_vfences.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_precomp_vfences$(PreprocessSuffix) "source/precomp_vfences.cpp"

$(IntermediateDirectory)/source_blockmodels_fences$(ObjectSuffix): source/blockmodels_fences.cpp $(IntermediateDirectory)/source_blockmodels_fences$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/blockmodels_fences.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_blockmodels_fences$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_blockmodels_fences$(DependSuffix): source/blockmodels_fences.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_blockmodels_fences$(ObjectSuffix) -MF$(IntermediateDirectory)/source_blockmodels_fences$(DependSuffix) -MM "source/blockmodels_fences.cpp"

$(IntermediateDirectory)/source_blockmodels_fences$(PreprocessSuffix): source/blockmodels_fences.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_blockmodels_fences$(PreprocessSuffix) "source/blockmodels_fences.cpp"

$(IntermediateDirectory)/source_precomp_vdoors$(ObjectSuffix): source/precomp_vdoors.cpp $(IntermediateDirectory)/source_precomp_vdoors$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/precomp_vdoors.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_precomp_vdoors$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_precomp_vdoors$(DependSuffix): source/precomp_vdoors.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_precomp_vdoors$(ObjectSuffix) -MF$(IntermediateDirectory)/source_precomp_vdoors$(DependSuffix) -MM "source/precomp_vdoors.cpp"

$(IntermediateDirectory)/source_precomp_vdoors$(PreprocessSuffix): source/precomp_vdoors.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_precomp_vdoors$(PreprocessSuffix) "source/precomp_vdoors.cpp"

$(IntermediateDirectory)/source_render_gui_playerhand$(ObjectSuffix): source/render_gui_playerhand.cpp $(IntermediateDirectory)/source_render_gui_playerhand$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/render_gui_playerhand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_render_gui_playerhand$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_render_gui_playerhand$(DependSuffix): source/render_gui_playerhand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_render_gui_playerhand$(ObjectSuffix) -MF$(IntermediateDirectory)/source_render_gui_playerhand$(DependSuffix) -MM "source/render_gui_playerhand.cpp"

$(IntermediateDirectory)/source_render_gui_playerhand$(PreprocessSuffix): source/render_gui_playerhand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_render_gui_playerhand$(PreprocessSuffix) "source/render_gui_playerhand.cpp"

$(IntermediateDirectory)/source_render_gui_quickbar$(ObjectSuffix): source/render_gui_quickbar.cpp $(IntermediateDirectory)/source_render_gui_quickbar$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/render_gui_quickbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_render_gui_quickbar$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_render_gui_quickbar$(DependSuffix): source/render_gui_quickbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_render_gui_quickbar$(ObjectSuffix) -MF$(IntermediateDirectory)/source_render_gui_quickbar$(DependSuffix) -MM "source/render_gui_quickbar.cpp"

$(IntermediateDirectory)/source_render_gui_quickbar$(PreprocessSuffix): source/render_gui_quickbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_render_gui_quickbar$(PreprocessSuffix) "source/render_gui_quickbar.cpp"

$(IntermediateDirectory)/source_render_player_selection$(ObjectSuffix): source/render_player_selection.cpp $(IntermediateDirectory)/source_render_player_selection$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/render_player_selection.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_render_player_selection$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_render_player_selection$(DependSuffix): source/render_player_selection.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_render_player_selection$(ObjectSuffix) -MF$(IntermediateDirectory)/source_render_player_selection$(DependSuffix) -MM "source/render_player_selection.cpp"

$(IntermediateDirectory)/source_render_player_selection$(PreprocessSuffix): source/render_player_selection.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_render_player_selection$(PreprocessSuffix) "source/render_player_selection.cpp"

$(IntermediateDirectory)/source_menu$(ObjectSuffix): source/menu.cpp $(IntermediateDirectory)/source_menu$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/menu.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_menu$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_menu$(DependSuffix): source/menu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_menu$(ObjectSuffix) -MF$(IntermediateDirectory)/source_menu$(DependSuffix) -MM "source/menu.cpp"

$(IntermediateDirectory)/source_menu$(PreprocessSuffix): source/menu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_menu$(PreprocessSuffix) "source/menu.cpp"

$(IntermediateDirectory)/source_player_actions_inputs$(ObjectSuffix): source/player_actions_inputs.cpp $(IntermediateDirectory)/source_player_actions_inputs$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/player_actions_inputs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_player_actions_inputs$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_player_actions_inputs$(DependSuffix): source/player_actions_inputs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_player_actions_inputs$(ObjectSuffix) -MF$(IntermediateDirectory)/source_player_actions_inputs$(DependSuffix) -MM "source/player_actions_inputs.cpp"

$(IntermediateDirectory)/source_player_actions_inputs$(PreprocessSuffix): source/player_actions_inputs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_player_actions_inputs$(PreprocessSuffix) "source/player_actions_inputs.cpp"

$(IntermediateDirectory)/source_player_actions_handlers$(ObjectSuffix): source/player_actions_handlers.cpp $(IntermediateDirectory)/source_player_actions_handlers$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/player_actions_handlers.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_player_actions_handlers$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_player_actions_handlers$(DependSuffix): source/player_actions_handlers.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_player_actions_handlers$(ObjectSuffix) -MF$(IntermediateDirectory)/source_player_actions_handlers$(DependSuffix) -MM "source/player_actions_handlers.cpp"

$(IntermediateDirectory)/source_player_actions_handlers$(PreprocessSuffix): source/player_actions_handlers.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_player_actions_handlers$(PreprocessSuffix) "source/player_actions_handlers.cpp"

$(IntermediateDirectory)/source_render_fsblur$(ObjectSuffix): source/render_fsblur.cpp $(IntermediateDirectory)/source_render_fsblur$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/render_fsblur.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_render_fsblur$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_render_fsblur$(DependSuffix): source/render_fsblur.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_render_fsblur$(ObjectSuffix) -MF$(IntermediateDirectory)/source_render_fsblur$(DependSuffix) -MM "source/render_fsblur.cpp"

$(IntermediateDirectory)/source_render_fsblur$(PreprocessSuffix): source/render_fsblur.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_render_fsblur$(PreprocessSuffix) "source/render_fsblur.cpp"

$(IntermediateDirectory)/source_camera$(ObjectSuffix): source/camera.cpp $(IntermediateDirectory)/source_camera$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/camera.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_camera$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_camera$(DependSuffix): source/camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_camera$(ObjectSuffix) -MF$(IntermediateDirectory)/source_camera$(DependSuffix) -MM "source/camera.cpp"

$(IntermediateDirectory)/source_camera$(PreprocessSuffix): source/camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_camera$(PreprocessSuffix) "source/camera.cpp"

$(IntermediateDirectory)/source_render_gui_inventory$(ObjectSuffix): source/render_gui_inventory.cpp $(IntermediateDirectory)/source_render_gui_inventory$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/render_gui_inventory.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_render_gui_inventory$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_render_gui_inventory$(DependSuffix): source/render_gui_inventory.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_render_gui_inventory$(ObjectSuffix) -MF$(IntermediateDirectory)/source_render_gui_inventory$(DependSuffix) -MM "source/render_gui_inventory.cpp"

$(IntermediateDirectory)/source_render_gui_inventory$(PreprocessSuffix): source/render_gui_inventory.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_render_gui_inventory$(PreprocessSuffix) "source/render_gui_inventory.cpp"

$(IntermediateDirectory)/source_worldmanager_init$(ObjectSuffix): source/worldmanager_init.cpp $(IntermediateDirectory)/source_worldmanager_init$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/worldmanager_init.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/source_worldmanager_init$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/source_worldmanager_init$(DependSuffix): source/worldmanager_init.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/source_worldmanager_init$(ObjectSuffix) -MF$(IntermediateDirectory)/source_worldmanager_init$(DependSuffix) -MM "source/worldmanager_init.cpp"

$(IntermediateDirectory)/source_worldmanager_init$(PreprocessSuffix): source/worldmanager_init.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/source_worldmanager_init$(PreprocessSuffix) "source/worldmanager_init.cpp"

$(IntermediateDirectory)/res_cppcraft.rc$(ObjectSuffix): res/cppcraft.rc
	$(RcCompilerName) -i "C:/Projects/cppcraft/res/cppcraft.rc" $(RcCmpOptions)   $(ObjectSwitch)$(IntermediateDirectory)/res_cppcraft.rc$(ObjectSuffix) $(RcIncludePath)
$(IntermediateDirectory)/library_config$(ObjectSuffix): source/library/config.cpp $(IntermediateDirectory)/library_config$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/library_config$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/library_config$(DependSuffix): source/library/config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/library_config$(ObjectSuffix) -MF$(IntermediateDirectory)/library_config$(DependSuffix) -MM "source/library/config.cpp"

$(IntermediateDirectory)/library_config$(PreprocessSuffix): source/library/config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/library_config$(PreprocessSuffix) "source/library/config.cpp"

$(IntermediateDirectory)/library_log$(ObjectSuffix): source/library/log.cpp $(IntermediateDirectory)/library_log$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/log.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/library_log$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/library_log$(DependSuffix): source/library/log.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/library_log$(ObjectSuffix) -MF$(IntermediateDirectory)/library_log$(DependSuffix) -MM "source/library/log.cpp"

$(IntermediateDirectory)/library_log$(PreprocessSuffix): source/library/log.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/library_log$(PreprocessSuffix) "source/library/log.cpp"

$(IntermediateDirectory)/tests_gltest$(ObjectSuffix): source/tests/gltest.cpp $(IntermediateDirectory)/tests_gltest$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/tests/gltest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tests_gltest$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tests_gltest$(DependSuffix): source/tests/gltest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tests_gltest$(ObjectSuffix) -MF$(IntermediateDirectory)/tests_gltest$(DependSuffix) -MM "source/tests/gltest.cpp"

$(IntermediateDirectory)/tests_gltest$(PreprocessSuffix): source/tests/gltest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tests_gltest$(PreprocessSuffix) "source/tests/gltest.cpp"

$(IntermediateDirectory)/tests_socktest$(ObjectSuffix): source/tests/socktest.cpp $(IntermediateDirectory)/tests_socktest$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/tests/socktest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tests_socktest$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tests_socktest$(DependSuffix): source/tests/socktest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tests_socktest$(ObjectSuffix) -MF$(IntermediateDirectory)/tests_socktest$(DependSuffix) -MM "source/tests/socktest.cpp"

$(IntermediateDirectory)/tests_socktest$(PreprocessSuffix): source/tests/socktest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tests_socktest$(PreprocessSuffix) "source/tests/socktest.cpp"

$(IntermediateDirectory)/tests_testmain$(ObjectSuffix): source/tests/testmain.cpp $(IntermediateDirectory)/tests_testmain$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/tests/testmain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tests_testmain$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tests_testmain$(DependSuffix): source/tests/testmain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tests_testmain$(ObjectSuffix) -MF$(IntermediateDirectory)/tests_testmain$(DependSuffix) -MM "source/tests/testmain.cpp"

$(IntermediateDirectory)/tests_testmain$(PreprocessSuffix): source/tests/testmain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tests_testmain$(PreprocessSuffix) "source/tests/testmain.cpp"

$(IntermediateDirectory)/tests_threadtest$(ObjectSuffix): source/tests/threadtest.cpp $(IntermediateDirectory)/tests_threadtest$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/tests/threadtest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tests_threadtest$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tests_threadtest$(DependSuffix): source/tests/threadtest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tests_threadtest$(ObjectSuffix) -MF$(IntermediateDirectory)/tests_threadtest$(DependSuffix) -MM "source/tests/threadtest.cpp"

$(IntermediateDirectory)/tests_threadtest$(PreprocessSuffix): source/tests/threadtest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tests_threadtest$(PreprocessSuffix) "source/tests/threadtest.cpp"

$(IntermediateDirectory)/tests_timetest$(ObjectSuffix): source/tests/timetest.cpp $(IntermediateDirectory)/tests_timetest$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/tests/timetest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tests_timetest$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tests_timetest$(DependSuffix): source/tests/timetest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tests_timetest$(ObjectSuffix) -MF$(IntermediateDirectory)/tests_timetest$(DependSuffix) -MM "source/tests/timetest.cpp"

$(IntermediateDirectory)/tests_timetest$(PreprocessSuffix): source/tests/timetest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tests_timetest$(PreprocessSuffix) "source/tests/timetest.cpp"

$(IntermediateDirectory)/bitmap_bitmap$(ObjectSuffix): source/library/bitmap/bitmap.cpp $(IntermediateDirectory)/bitmap_bitmap$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/bitmap/bitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bitmap_bitmap$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bitmap_bitmap$(DependSuffix): source/library/bitmap/bitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bitmap_bitmap$(ObjectSuffix) -MF$(IntermediateDirectory)/bitmap_bitmap$(DependSuffix) -MM "source/library/bitmap/bitmap.cpp"

$(IntermediateDirectory)/bitmap_bitmap$(PreprocessSuffix): source/library/bitmap/bitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bitmap_bitmap$(PreprocessSuffix) "source/library/bitmap/bitmap.cpp"

$(IntermediateDirectory)/bitmap_colortools$(ObjectSuffix): source/library/bitmap/colortools.cpp $(IntermediateDirectory)/bitmap_colortools$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/bitmap/colortools.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bitmap_colortools$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bitmap_colortools$(DependSuffix): source/library/bitmap/colortools.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bitmap_colortools$(ObjectSuffix) -MF$(IntermediateDirectory)/bitmap_colortools$(DependSuffix) -MM "source/library/bitmap/colortools.cpp"

$(IntermediateDirectory)/bitmap_colortools$(PreprocessSuffix): source/library/bitmap/colortools.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bitmap_colortools$(PreprocessSuffix) "source/library/bitmap/colortools.cpp"

$(IntermediateDirectory)/bitmap_lodepng$(ObjectSuffix): source/library/bitmap/lodepng.cpp $(IntermediateDirectory)/bitmap_lodepng$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/bitmap/lodepng.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bitmap_lodepng$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bitmap_lodepng$(DependSuffix): source/library/bitmap/lodepng.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bitmap_lodepng$(ObjectSuffix) -MF$(IntermediateDirectory)/bitmap_lodepng$(DependSuffix) -MM "source/library/bitmap/lodepng.cpp"

$(IntermediateDirectory)/bitmap_lodepng$(PreprocessSuffix): source/library/bitmap/lodepng.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bitmap_lodepng$(PreprocessSuffix) "source/library/bitmap/lodepng.cpp"

$(IntermediateDirectory)/compression_lzo$(ObjectSuffix): source/library/compression/lzo.cpp $(IntermediateDirectory)/compression_lzo$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/compression/lzo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/compression_lzo$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/compression_lzo$(DependSuffix): source/library/compression/lzo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/compression_lzo$(ObjectSuffix) -MF$(IntermediateDirectory)/compression_lzo$(DependSuffix) -MM "source/library/compression/lzo.cpp"

$(IntermediateDirectory)/compression_lzo$(PreprocessSuffix): source/library/compression/lzo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/compression_lzo$(PreprocessSuffix) "source/library/compression/lzo.cpp"

$(IntermediateDirectory)/math_baseconv$(ObjectSuffix): source/library/math/baseconv.cpp $(IntermediateDirectory)/math_baseconv$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/math/baseconv.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/math_baseconv$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/math_baseconv$(DependSuffix): source/library/math/baseconv.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/math_baseconv$(ObjectSuffix) -MF$(IntermediateDirectory)/math_baseconv$(DependSuffix) -MM "source/library/math/baseconv.cpp"

$(IntermediateDirectory)/math_baseconv$(PreprocessSuffix): source/library/math/baseconv.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/math_baseconv$(PreprocessSuffix) "source/library/math/baseconv.cpp"

$(IntermediateDirectory)/math_frustum$(ObjectSuffix): source/library/math/frustum.cpp $(IntermediateDirectory)/math_frustum$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/math/frustum.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/math_frustum$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/math_frustum$(DependSuffix): source/library/math/frustum.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/math_frustum$(ObjectSuffix) -MF$(IntermediateDirectory)/math_frustum$(DependSuffix) -MM "source/library/math/frustum.cpp"

$(IntermediateDirectory)/math_frustum$(PreprocessSuffix): source/library/math/frustum.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/math_frustum$(PreprocessSuffix) "source/library/math/frustum.cpp"

$(IntermediateDirectory)/math_matrix$(ObjectSuffix): source/library/math/matrix.cpp $(IntermediateDirectory)/math_matrix$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/math/matrix.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/math_matrix$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/math_matrix$(DependSuffix): source/library/math/matrix.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/math_matrix$(ObjectSuffix) -MF$(IntermediateDirectory)/math_matrix$(DependSuffix) -MM "source/library/math/matrix.cpp"

$(IntermediateDirectory)/math_matrix$(PreprocessSuffix): source/library/math/matrix.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/math_matrix$(PreprocessSuffix) "source/library/math/matrix.cpp"

$(IntermediateDirectory)/math_quaternion$(ObjectSuffix): source/library/math/quaternion.cpp $(IntermediateDirectory)/math_quaternion$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/math/quaternion.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/math_quaternion$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/math_quaternion$(DependSuffix): source/library/math/quaternion.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/math_quaternion$(ObjectSuffix) -MF$(IntermediateDirectory)/math_quaternion$(DependSuffix) -MM "source/library/math/quaternion.cpp"

$(IntermediateDirectory)/math_quaternion$(PreprocessSuffix): source/library/math/quaternion.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/math_quaternion$(PreprocessSuffix) "source/library/math/quaternion.cpp"

$(IntermediateDirectory)/math_vector$(ObjectSuffix): source/library/math/vector.cpp $(IntermediateDirectory)/math_vector$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/math/vector.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/math_vector$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/math_vector$(DependSuffix): source/library/math/vector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/math_vector$(ObjectSuffix) -MF$(IntermediateDirectory)/math_vector$(DependSuffix) -MM "source/library/math/vector.cpp"

$(IntermediateDirectory)/math_vector$(PreprocessSuffix): source/library/math/vector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/math_vector$(PreprocessSuffix) "source/library/math/vector.cpp"

$(IntermediateDirectory)/math_voronoi$(ObjectSuffix): source/library/math/voronoi.cpp $(IntermediateDirectory)/math_voronoi$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/math/voronoi.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/math_voronoi$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/math_voronoi$(DependSuffix): source/library/math/voronoi.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/math_voronoi$(ObjectSuffix) -MF$(IntermediateDirectory)/math_voronoi$(DependSuffix) -MM "source/library/math/voronoi.cpp"

$(IntermediateDirectory)/math_voronoi$(PreprocessSuffix): source/library/math/voronoi.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/math_voronoi$(PreprocessSuffix) "source/library/math/voronoi.cpp"

$(IntermediateDirectory)/math_vec3$(ObjectSuffix): source/library/math/vec3.cpp $(IntermediateDirectory)/math_vec3$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/math/vec3.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/math_vec3$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/math_vec3$(DependSuffix): source/library/math/vec3.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/math_vec3$(ObjectSuffix) -MF$(IntermediateDirectory)/math_vec3$(DependSuffix) -MM "source/library/math/vec3.cpp"

$(IntermediateDirectory)/math_vec3$(PreprocessSuffix): source/library/math/vec3.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/math_vec3$(PreprocessSuffix) "source/library/math/vec3.cpp"

$(IntermediateDirectory)/math_vec4$(ObjectSuffix): source/library/math/vec4.cpp $(IntermediateDirectory)/math_vec4$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/math/vec4.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/math_vec4$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/math_vec4$(DependSuffix): source/library/math/vec4.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/math_vec4$(ObjectSuffix) -MF$(IntermediateDirectory)/math_vec4$(DependSuffix) -MM "source/library/math/vec4.cpp"

$(IntermediateDirectory)/math_vec4$(PreprocessSuffix): source/library/math/vec4.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/math_vec4$(PreprocessSuffix) "source/library/math/vec4.cpp"

$(IntermediateDirectory)/network_hostaddr$(ObjectSuffix): source/library/network/hostaddr.cpp $(IntermediateDirectory)/network_hostaddr$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/network/hostaddr.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/network_hostaddr$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/network_hostaddr$(DependSuffix): source/library/network/hostaddr.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/network_hostaddr$(ObjectSuffix) -MF$(IntermediateDirectory)/network_hostaddr$(DependSuffix) -MM "source/library/network/hostaddr.cpp"

$(IntermediateDirectory)/network_hostaddr$(PreprocessSuffix): source/library/network/hostaddr.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/network_hostaddr$(PreprocessSuffix) "source/library/network/hostaddr.cpp"

$(IntermediateDirectory)/network_socket$(ObjectSuffix): source/library/network/socket.cpp $(IntermediateDirectory)/network_socket$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/network/socket.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/network_socket$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/network_socket$(DependSuffix): source/library/network/socket.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/network_socket$(ObjectSuffix) -MF$(IntermediateDirectory)/network_socket$(DependSuffix) -MM "source/library/network/socket.cpp"

$(IntermediateDirectory)/network_socket$(PreprocessSuffix): source/library/network/socket.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/network_socket$(PreprocessSuffix) "source/library/network/socket.cpp"

$(IntermediateDirectory)/network_tcpsocket$(ObjectSuffix): source/library/network/tcpsocket.cpp $(IntermediateDirectory)/network_tcpsocket$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/network/tcpsocket.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/network_tcpsocket$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/network_tcpsocket$(DependSuffix): source/library/network/tcpsocket.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/network_tcpsocket$(ObjectSuffix) -MF$(IntermediateDirectory)/network_tcpsocket$(DependSuffix) -MM "source/library/network/tcpsocket.cpp"

$(IntermediateDirectory)/network_tcpsocket$(PreprocessSuffix): source/library/network/tcpsocket.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/network_tcpsocket$(PreprocessSuffix) "source/library/network/tcpsocket.cpp"

$(IntermediateDirectory)/noise_cosnoise$(ObjectSuffix): source/library/noise/cosnoise.cpp $(IntermediateDirectory)/noise_cosnoise$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/noise/cosnoise.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/noise_cosnoise$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/noise_cosnoise$(DependSuffix): source/library/noise/cosnoise.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/noise_cosnoise$(ObjectSuffix) -MF$(IntermediateDirectory)/noise_cosnoise$(DependSuffix) -MM "source/library/noise/cosnoise.cpp"

$(IntermediateDirectory)/noise_cosnoise$(PreprocessSuffix): source/library/noise/cosnoise.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/noise_cosnoise$(PreprocessSuffix) "source/library/noise/cosnoise.cpp"

$(IntermediateDirectory)/noise_gabor$(ObjectSuffix): source/library/noise/gabor.c $(IntermediateDirectory)/noise_gabor$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/cppcraft/source/library/noise/gabor.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/noise_gabor$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/noise_gabor$(DependSuffix): source/library/noise/gabor.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/noise_gabor$(ObjectSuffix) -MF$(IntermediateDirectory)/noise_gabor$(DependSuffix) -MM "source/library/noise/gabor.c"

$(IntermediateDirectory)/noise_gabor$(PreprocessSuffix): source/library/noise/gabor.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/noise_gabor$(PreprocessSuffix) "source/library/noise/gabor.c"

$(IntermediateDirectory)/noise_multifractal$(ObjectSuffix): source/library/noise/multifractal.c $(IntermediateDirectory)/noise_multifractal$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/cppcraft/source/library/noise/multifractal.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/noise_multifractal$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/noise_multifractal$(DependSuffix): source/library/noise/multifractal.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/noise_multifractal$(ObjectSuffix) -MF$(IntermediateDirectory)/noise_multifractal$(DependSuffix) -MM "source/library/noise/multifractal.c"

$(IntermediateDirectory)/noise_multifractal$(PreprocessSuffix): source/library/noise/multifractal.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/noise_multifractal$(PreprocessSuffix) "source/library/noise/multifractal.c"

$(IntermediateDirectory)/noise_noise1234$(ObjectSuffix): source/library/noise/noise1234.c $(IntermediateDirectory)/noise_noise1234$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/cppcraft/source/library/noise/noise1234.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/noise_noise1234$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/noise_noise1234$(DependSuffix): source/library/noise/noise1234.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/noise_noise1234$(ObjectSuffix) -MF$(IntermediateDirectory)/noise_noise1234$(DependSuffix) -MM "source/library/noise/noise1234.c"

$(IntermediateDirectory)/noise_noise1234$(PreprocessSuffix): source/library/noise/noise1234.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/noise_noise1234$(PreprocessSuffix) "source/library/noise/noise1234.c"

$(IntermediateDirectory)/noise_sdnoise1234$(ObjectSuffix): source/library/noise/sdnoise1234.c $(IntermediateDirectory)/noise_sdnoise1234$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/cppcraft/source/library/noise/sdnoise1234.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/noise_sdnoise1234$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/noise_sdnoise1234$(DependSuffix): source/library/noise/sdnoise1234.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/noise_sdnoise1234$(ObjectSuffix) -MF$(IntermediateDirectory)/noise_sdnoise1234$(DependSuffix) -MM "source/library/noise/sdnoise1234.c"

$(IntermediateDirectory)/noise_sdnoise1234$(PreprocessSuffix): source/library/noise/sdnoise1234.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/noise_sdnoise1234$(PreprocessSuffix) "source/library/noise/sdnoise1234.c"

$(IntermediateDirectory)/noise_simplex1234$(ObjectSuffix): source/library/noise/simplex1234.c $(IntermediateDirectory)/noise_simplex1234$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/cppcraft/source/library/noise/simplex1234.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/noise_simplex1234$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/noise_simplex1234$(DependSuffix): source/library/noise/simplex1234.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/noise_simplex1234$(ObjectSuffix) -MF$(IntermediateDirectory)/noise_simplex1234$(DependSuffix) -MM "source/library/noise/simplex1234.c"

$(IntermediateDirectory)/noise_simplex1234$(PreprocessSuffix): source/library/noise/simplex1234.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/noise_simplex1234$(PreprocessSuffix) "source/library/noise/simplex1234.c"

$(IntermediateDirectory)/noise_srdnoise23$(ObjectSuffix): source/library/noise/srdnoise23.c $(IntermediateDirectory)/noise_srdnoise23$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/cppcraft/source/library/noise/srdnoise23.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/noise_srdnoise23$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/noise_srdnoise23$(DependSuffix): source/library/noise/srdnoise23.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/noise_srdnoise23$(ObjectSuffix) -MF$(IntermediateDirectory)/noise_srdnoise23$(DependSuffix) -MM "source/library/noise/srdnoise23.c"

$(IntermediateDirectory)/noise_srdnoise23$(PreprocessSuffix): source/library/noise/srdnoise23.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/noise_srdnoise23$(PreprocessSuffix) "source/library/noise/srdnoise23.c"

$(IntermediateDirectory)/noise_torusnoise$(ObjectSuffix): source/library/noise/torusnoise.c $(IntermediateDirectory)/noise_torusnoise$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/cppcraft/source/library/noise/torusnoise.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/noise_torusnoise$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/noise_torusnoise$(DependSuffix): source/library/noise/torusnoise.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/noise_torusnoise$(ObjectSuffix) -MF$(IntermediateDirectory)/noise_torusnoise$(DependSuffix) -MM "source/library/noise/torusnoise.c"

$(IntermediateDirectory)/noise_torusnoise$(PreprocessSuffix): source/library/noise/torusnoise.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/noise_torusnoise$(PreprocessSuffix) "source/library/noise/torusnoise.c"

$(IntermediateDirectory)/noise_turbulence$(ObjectSuffix): source/library/noise/turbulence.c $(IntermediateDirectory)/noise_turbulence$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/cppcraft/source/library/noise/turbulence.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/noise_turbulence$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/noise_turbulence$(DependSuffix): source/library/noise/turbulence.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/noise_turbulence$(ObjectSuffix) -MF$(IntermediateDirectory)/noise_turbulence$(DependSuffix) -MM "source/library/noise/turbulence.c"

$(IntermediateDirectory)/noise_turbulence$(PreprocessSuffix): source/library/noise/turbulence.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/noise_turbulence$(PreprocessSuffix) "source/library/noise/turbulence.c"

$(IntermediateDirectory)/noise_worley$(ObjectSuffix): source/library/noise/worley.c $(IntermediateDirectory)/noise_worley$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Projects/cppcraft/source/library/noise/worley.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/noise_worley$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/noise_worley$(DependSuffix): source/library/noise/worley.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/noise_worley$(ObjectSuffix) -MF$(IntermediateDirectory)/noise_worley$(DependSuffix) -MM "source/library/noise/worley.c"

$(IntermediateDirectory)/noise_worley$(PreprocessSuffix): source/library/noise/worley.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/noise_worley$(PreprocessSuffix) "source/library/noise/worley.c"

$(IntermediateDirectory)/opengl_oglfont$(ObjectSuffix): source/library/opengl/oglfont.cpp $(IntermediateDirectory)/opengl_oglfont$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/opengl/oglfont.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/opengl_oglfont$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/opengl_oglfont$(DependSuffix): source/library/opengl/oglfont.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/opengl_oglfont$(ObjectSuffix) -MF$(IntermediateDirectory)/opengl_oglfont$(DependSuffix) -MM "source/library/opengl/oglfont.cpp"

$(IntermediateDirectory)/opengl_oglfont$(PreprocessSuffix): source/library/opengl/oglfont.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/opengl_oglfont$(PreprocessSuffix) "source/library/opengl/oglfont.cpp"

$(IntermediateDirectory)/opengl_opengl$(ObjectSuffix): source/library/opengl/opengl.cpp $(IntermediateDirectory)/opengl_opengl$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/opengl/opengl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/opengl_opengl$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/opengl_opengl$(DependSuffix): source/library/opengl/opengl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/opengl_opengl$(ObjectSuffix) -MF$(IntermediateDirectory)/opengl_opengl$(DependSuffix) -MM "source/library/opengl/opengl.cpp"

$(IntermediateDirectory)/opengl_opengl$(PreprocessSuffix): source/library/opengl/opengl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/opengl_opengl$(PreprocessSuffix) "source/library/opengl/opengl.cpp"

$(IntermediateDirectory)/opengl_openglext$(ObjectSuffix): source/library/opengl/openglext.cpp $(IntermediateDirectory)/opengl_openglext$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/opengl/openglext.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/opengl_openglext$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/opengl_openglext$(DependSuffix): source/library/opengl/openglext.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/opengl_openglext$(ObjectSuffix) -MF$(IntermediateDirectory)/opengl_openglext$(DependSuffix) -MM "source/library/opengl/openglext.cpp"

$(IntermediateDirectory)/opengl_openglext$(PreprocessSuffix): source/library/opengl/openglext.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/opengl_openglext$(PreprocessSuffix) "source/library/opengl/openglext.cpp"

$(IntermediateDirectory)/opengl_shader$(ObjectSuffix): source/library/opengl/shader.cpp $(IntermediateDirectory)/opengl_shader$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/opengl/shader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/opengl_shader$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/opengl_shader$(DependSuffix): source/library/opengl/shader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/opengl_shader$(ObjectSuffix) -MF$(IntermediateDirectory)/opengl_shader$(DependSuffix) -MM "source/library/opengl/shader.cpp"

$(IntermediateDirectory)/opengl_shader$(PreprocessSuffix): source/library/opengl/shader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/opengl_shader$(PreprocessSuffix) "source/library/opengl/shader.cpp"

$(IntermediateDirectory)/opengl_texture$(ObjectSuffix): source/library/opengl/texture.cpp $(IntermediateDirectory)/opengl_texture$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/opengl/texture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/opengl_texture$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/opengl_texture$(DependSuffix): source/library/opengl/texture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/opengl_texture$(ObjectSuffix) -MF$(IntermediateDirectory)/opengl_texture$(DependSuffix) -MM "source/library/opengl/texture.cpp"

$(IntermediateDirectory)/opengl_texture$(PreprocessSuffix): source/library/opengl/texture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/opengl_texture$(PreprocessSuffix) "source/library/opengl/texture.cpp"

$(IntermediateDirectory)/opengl_window$(ObjectSuffix): source/library/opengl/window.cpp $(IntermediateDirectory)/opengl_window$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/opengl/window.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/opengl_window$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/opengl_window$(DependSuffix): source/library/opengl/window.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/opengl_window$(ObjectSuffix) -MF$(IntermediateDirectory)/opengl_window$(DependSuffix) -MM "source/library/opengl/window.cpp"

$(IntermediateDirectory)/opengl_window$(PreprocessSuffix): source/library/opengl/window.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/opengl_window$(PreprocessSuffix) "source/library/opengl/window.cpp"

$(IntermediateDirectory)/opengl_fbo$(ObjectSuffix): source/library/opengl/fbo.cpp $(IntermediateDirectory)/opengl_fbo$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/opengl/fbo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/opengl_fbo$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/opengl_fbo$(DependSuffix): source/library/opengl/fbo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/opengl_fbo$(ObjectSuffix) -MF$(IntermediateDirectory)/opengl_fbo$(DependSuffix) -MM "source/library/opengl/fbo.cpp"

$(IntermediateDirectory)/opengl_fbo$(PreprocessSuffix): source/library/opengl/fbo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/opengl_fbo$(PreprocessSuffix) "source/library/opengl/fbo.cpp"

$(IntermediateDirectory)/opengl_vao$(ObjectSuffix): source/library/opengl/vao.cpp $(IntermediateDirectory)/opengl_vao$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/opengl/vao.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/opengl_vao$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/opengl_vao$(DependSuffix): source/library/opengl/vao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/opengl_vao$(ObjectSuffix) -MF$(IntermediateDirectory)/opengl_vao$(DependSuffix) -MM "source/library/opengl/vao.cpp"

$(IntermediateDirectory)/opengl_vao$(PreprocessSuffix): source/library/opengl/vao.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/opengl_vao$(PreprocessSuffix) "source/library/opengl/vao.cpp"

$(IntermediateDirectory)/sound_sound$(ObjectSuffix): source/library/sound/sound.cpp $(IntermediateDirectory)/sound_sound$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/sound/sound.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sound_sound$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sound_sound$(DependSuffix): source/library/sound/sound.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sound_sound$(ObjectSuffix) -MF$(IntermediateDirectory)/sound_sound$(DependSuffix) -MM "source/library/sound/sound.cpp"

$(IntermediateDirectory)/sound_sound$(PreprocessSuffix): source/library/sound/sound.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sound_sound$(PreprocessSuffix) "source/library/sound/sound.cpp"

$(IntermediateDirectory)/sound_stream_channel$(ObjectSuffix): source/library/sound/stream_channel.cpp $(IntermediateDirectory)/sound_stream_channel$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/sound/stream_channel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sound_stream_channel$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sound_stream_channel$(DependSuffix): source/library/sound/stream_channel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sound_stream_channel$(ObjectSuffix) -MF$(IntermediateDirectory)/sound_stream_channel$(DependSuffix) -MM "source/library/sound/stream_channel.cpp"

$(IntermediateDirectory)/sound_stream_channel$(PreprocessSuffix): source/library/sound/stream_channel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sound_stream_channel$(PreprocessSuffix) "source/library/sound/stream_channel.cpp"

$(IntermediateDirectory)/sound_stream$(ObjectSuffix): source/library/sound/stream.cpp $(IntermediateDirectory)/sound_stream$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/sound/stream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sound_stream$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sound_stream$(DependSuffix): source/library/sound/stream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sound_stream$(ObjectSuffix) -MF$(IntermediateDirectory)/sound_stream$(DependSuffix) -MM "source/library/sound/stream.cpp"

$(IntermediateDirectory)/sound_stream$(PreprocessSuffix): source/library/sound/stream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sound_stream$(PreprocessSuffix) "source/library/sound/stream.cpp"

$(IntermediateDirectory)/threading_thread_controller$(ObjectSuffix): source/library/threading/thread_controller.cpp $(IntermediateDirectory)/threading_thread_controller$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/threading/thread_controller.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/threading_thread_controller$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/threading_thread_controller$(DependSuffix): source/library/threading/thread_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/threading_thread_controller$(ObjectSuffix) -MF$(IntermediateDirectory)/threading_thread_controller$(DependSuffix) -MM "source/library/threading/thread_controller.cpp"

$(IntermediateDirectory)/threading_thread_controller$(PreprocessSuffix): source/library/threading/thread_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/threading_thread_controller$(PreprocessSuffix) "source/library/threading/thread_controller.cpp"

$(IntermediateDirectory)/threading_thread_worker$(ObjectSuffix): source/library/threading/thread_worker.cpp $(IntermediateDirectory)/threading_thread_worker$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/threading/thread_worker.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/threading_thread_worker$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/threading_thread_worker$(DependSuffix): source/library/threading/thread_worker.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/threading_thread_worker$(ObjectSuffix) -MF$(IntermediateDirectory)/threading_thread_worker$(DependSuffix) -MM "source/library/threading/thread_worker.cpp"

$(IntermediateDirectory)/threading_thread_worker$(PreprocessSuffix): source/library/threading/thread_worker.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/threading_thread_worker$(PreprocessSuffix) "source/library/threading/thread_worker.cpp"

$(IntermediateDirectory)/threading_thread_pool$(ObjectSuffix): source/library/threading/thread_pool.cpp $(IntermediateDirectory)/threading_thread_pool$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/threading/thread_pool.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/threading_thread_pool$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/threading_thread_pool$(DependSuffix): source/library/threading/thread_pool.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/threading_thread_pool$(ObjectSuffix) -MF$(IntermediateDirectory)/threading_thread_pool$(DependSuffix) -MM "source/library/threading/thread_pool.cpp"

$(IntermediateDirectory)/threading_thread_pool$(PreprocessSuffix): source/library/threading/thread_pool.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/threading_thread_pool$(PreprocessSuffix) "source/library/threading/thread_pool.cpp"

$(IntermediateDirectory)/timing_highperf$(ObjectSuffix): source/library/timing/highperf.cpp $(IntermediateDirectory)/timing_highperf$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/timing/highperf.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/timing_highperf$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/timing_highperf$(DependSuffix): source/library/timing/highperf.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/timing_highperf$(ObjectSuffix) -MF$(IntermediateDirectory)/timing_highperf$(DependSuffix) -MM "source/library/timing/highperf.cpp"

$(IntermediateDirectory)/timing_highperf$(PreprocessSuffix): source/library/timing/highperf.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/timing_highperf$(PreprocessSuffix) "source/library/timing/highperf.cpp"

$(IntermediateDirectory)/timing_timer$(ObjectSuffix): source/library/timing/timer.cpp $(IntermediateDirectory)/timing_timer$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/timing/timer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/timing_timer$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/timing_timer$(DependSuffix): source/library/timing/timer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/timing_timer$(ObjectSuffix) -MF$(IntermediateDirectory)/timing_timer$(DependSuffix) -MM "source/library/timing/timer.cpp"

$(IntermediateDirectory)/timing_timer$(PreprocessSuffix): source/library/timing/timer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/timing_timer$(PreprocessSuffix) "source/library/timing/timer.cpp"

$(IntermediateDirectory)/voxels_voxelizer$(ObjectSuffix): source/library/voxels/voxelizer.cpp $(IntermediateDirectory)/voxels_voxelizer$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/voxels/voxelizer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/voxels_voxelizer$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/voxels_voxelizer$(DependSuffix): source/library/voxels/voxelizer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/voxels_voxelizer$(ObjectSuffix) -MF$(IntermediateDirectory)/voxels_voxelizer$(DependSuffix) -MM "source/library/voxels/voxelizer.cpp"

$(IntermediateDirectory)/voxels_voxelizer$(PreprocessSuffix): source/library/voxels/voxelizer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/voxels_voxelizer$(PreprocessSuffix) "source/library/voxels/voxelizer.cpp"

$(IntermediateDirectory)/voxels_voxelmodel$(ObjectSuffix): source/library/voxels/voxelmodel.cpp $(IntermediateDirectory)/voxels_voxelmodel$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Projects/cppcraft/source/library/voxels/voxelmodel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/voxels_voxelmodel$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/voxels_voxelmodel$(DependSuffix): source/library/voxels/voxelmodel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/voxels_voxelmodel$(ObjectSuffix) -MF$(IntermediateDirectory)/voxels_voxelmodel$(DependSuffix) -MM "source/library/voxels/voxelmodel.cpp"

$(IntermediateDirectory)/voxels_voxelmodel$(PreprocessSuffix): source/library/voxels/voxelmodel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/voxels_voxelmodel$(PreprocessSuffix) "source/library/voxels/voxelmodel.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/source_atmosphere$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_atmosphere$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_atmosphere$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_biome$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_biome$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_biome$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_crosses$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_crosses$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_crosses$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_cubes$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_cubes$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_cubes$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_blocks$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_blocks$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_blocks$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_chat$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_chat$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_chat$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_chunks$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_chunks$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_chunks$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_columns$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_columns$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_columns$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_compilers$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_compilers$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_compilers$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_compressor$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_compressor$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_compressor$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_drawq$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_drawq$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_drawq$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_flatlands$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_flatlands$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_flatlands$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_generator$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_generator$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_generator$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_items$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_items$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_items$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_lighting$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_lighting$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_lighting$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_lighttable$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_lighttable$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_lighttable$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_minimap$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_minimap$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_minimap$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_player$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_player$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_player$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_precompiler$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_precompiler$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_precompiler$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_precompq$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_precompq$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_precompq$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_thread$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_thread$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_thread$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_thread_process$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_thread_process$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_thread_process$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vemitter$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vemitter$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vemitter$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_rendergrid$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_rendergrid$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_rendergrid$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_renderman$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_renderman$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_renderman$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_seamless$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_seamless$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_seamless$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_sector$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_sector$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_sector$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_sectors$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_sectors$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_sectors$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_shaderman$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_shaderman$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_shaderman$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_soundman$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_soundman$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_soundman$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_spiders$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_spiders$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_spiders$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_sun$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_sun$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_sun$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_textureman$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_textureman$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_textureman$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_threading$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_threading$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_threading$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_tiles$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_tiles$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_tiles$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_torchlight$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_torchlight$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_torchlight$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_voxelmodels$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_voxelmodels$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_voxelmodels$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_world$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_world$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_world$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_worldbuilder$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_worldbuilder$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_worldbuilder$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_worldmanager$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_worldmanager$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_worldmanager$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_render_scene$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_render_scene$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_render_scene$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_render_sky$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_render_sky$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_render_sky$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_input$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_input$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_input$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_render_sky_clouds$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_render_sky_clouds$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_render_sky_clouds$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_player_inputs$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_player_inputs$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_player_inputs$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vcrosses$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vcrosses$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vcrosses$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_thread_ao$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_thread_ao$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_thread_ao$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_render_fsflare$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_render_fsflare$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_render_fsflare$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_render_fs$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_render_fs$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_render_fs$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_torchlight_calc$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_torchlight_calc$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_torchlight_calc$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_torchlight_gatherer$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_torchlight_gatherer$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_torchlight_gatherer$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_particles$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_particles$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_particles$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_particles_render$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_particles_render$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_particles_render$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_player_translate$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_player_translate$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_player_translate$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_render_scene_camera$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_render_scene_camera$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_render_scene_camera$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_player_actions$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_player_actions$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_player_actions$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_leafs$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_leafs$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_leafs$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_render_scene_terrain$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_render_scene_terrain$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_render_scene_terrain$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_spiders_modify$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_spiders_modify$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_spiders_modify$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_player_logic$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_player_logic$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_player_logic$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_player_physics$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_player_physics$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_player_physics$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui_crosshair$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui_crosshair$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui_crosshair$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_textureman_generated$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_textureman_generated$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_textureman_generated$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_spiders_world$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_spiders_world$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_spiders_world$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_ladders$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_ladders$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_ladders$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_lanterns$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_lanterns$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_lanterns$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_poles$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_poles$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_poles$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vladders$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vladders$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vladders$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vlantern$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vlantern$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vlantern$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vpoles$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vpoles$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vpoles$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vsloped$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vsloped$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vsloped$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui_compass$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui_compass$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui_compass$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vstairs$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vstairs$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vstairs$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_stairs$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_stairs$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_stairs$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vfences$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vfences$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vfences$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_fences$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_fences$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_blockmodels_fences$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vdoors$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vdoors$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_precomp_vdoors$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui_playerhand$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui_playerhand$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui_playerhand$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui_quickbar$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui_quickbar$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui_quickbar$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_render_player_selection$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_render_player_selection$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_render_player_selection$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_menu$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_menu$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_menu$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_player_actions_inputs$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_player_actions_inputs$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_player_actions_inputs$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_player_actions_handlers$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_player_actions_handlers$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_player_actions_handlers$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_render_fsblur$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_render_fsblur$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_render_fsblur$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_camera$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_camera$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_camera$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui_inventory$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui_inventory$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_render_gui_inventory$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/source_worldmanager_init$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/source_worldmanager_init$(DependSuffix)
	$(RM) $(IntermediateDirectory)/source_worldmanager_init$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/cppcraft.rc$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/library_config$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/library_config$(DependSuffix)
	$(RM) $(IntermediateDirectory)/library_config$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/library_log$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/library_log$(DependSuffix)
	$(RM) $(IntermediateDirectory)/library_log$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/tests_gltest$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/tests_gltest$(DependSuffix)
	$(RM) $(IntermediateDirectory)/tests_gltest$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/tests_socktest$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/tests_socktest$(DependSuffix)
	$(RM) $(IntermediateDirectory)/tests_socktest$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/tests_testmain$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/tests_testmain$(DependSuffix)
	$(RM) $(IntermediateDirectory)/tests_testmain$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/tests_threadtest$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/tests_threadtest$(DependSuffix)
	$(RM) $(IntermediateDirectory)/tests_threadtest$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/tests_timetest$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/tests_timetest$(DependSuffix)
	$(RM) $(IntermediateDirectory)/tests_timetest$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/bitmap_bitmap$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/bitmap_bitmap$(DependSuffix)
	$(RM) $(IntermediateDirectory)/bitmap_bitmap$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/bitmap_colortools$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/bitmap_colortools$(DependSuffix)
	$(RM) $(IntermediateDirectory)/bitmap_colortools$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/bitmap_lodepng$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/bitmap_lodepng$(DependSuffix)
	$(RM) $(IntermediateDirectory)/bitmap_lodepng$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/compression_lzo$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/compression_lzo$(DependSuffix)
	$(RM) $(IntermediateDirectory)/compression_lzo$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/math_baseconv$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/math_baseconv$(DependSuffix)
	$(RM) $(IntermediateDirectory)/math_baseconv$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/math_frustum$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/math_frustum$(DependSuffix)
	$(RM) $(IntermediateDirectory)/math_frustum$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/math_matrix$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/math_matrix$(DependSuffix)
	$(RM) $(IntermediateDirectory)/math_matrix$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/math_quaternion$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/math_quaternion$(DependSuffix)
	$(RM) $(IntermediateDirectory)/math_quaternion$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/math_vector$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/math_vector$(DependSuffix)
	$(RM) $(IntermediateDirectory)/math_vector$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/math_voronoi$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/math_voronoi$(DependSuffix)
	$(RM) $(IntermediateDirectory)/math_voronoi$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/math_vec3$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/math_vec3$(DependSuffix)
	$(RM) $(IntermediateDirectory)/math_vec3$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/math_vec4$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/math_vec4$(DependSuffix)
	$(RM) $(IntermediateDirectory)/math_vec4$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/network_hostaddr$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/network_hostaddr$(DependSuffix)
	$(RM) $(IntermediateDirectory)/network_hostaddr$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/network_socket$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/network_socket$(DependSuffix)
	$(RM) $(IntermediateDirectory)/network_socket$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/network_tcpsocket$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/network_tcpsocket$(DependSuffix)
	$(RM) $(IntermediateDirectory)/network_tcpsocket$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/noise_cosnoise$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/noise_cosnoise$(DependSuffix)
	$(RM) $(IntermediateDirectory)/noise_cosnoise$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/noise_gabor$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/noise_gabor$(DependSuffix)
	$(RM) $(IntermediateDirectory)/noise_gabor$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/noise_multifractal$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/noise_multifractal$(DependSuffix)
	$(RM) $(IntermediateDirectory)/noise_multifractal$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/noise_noise1234$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/noise_noise1234$(DependSuffix)
	$(RM) $(IntermediateDirectory)/noise_noise1234$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/noise_sdnoise1234$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/noise_sdnoise1234$(DependSuffix)
	$(RM) $(IntermediateDirectory)/noise_sdnoise1234$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/noise_simplex1234$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/noise_simplex1234$(DependSuffix)
	$(RM) $(IntermediateDirectory)/noise_simplex1234$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/noise_srdnoise23$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/noise_srdnoise23$(DependSuffix)
	$(RM) $(IntermediateDirectory)/noise_srdnoise23$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/noise_torusnoise$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/noise_torusnoise$(DependSuffix)
	$(RM) $(IntermediateDirectory)/noise_torusnoise$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/noise_turbulence$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/noise_turbulence$(DependSuffix)
	$(RM) $(IntermediateDirectory)/noise_turbulence$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/noise_worley$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/noise_worley$(DependSuffix)
	$(RM) $(IntermediateDirectory)/noise_worley$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/opengl_oglfont$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/opengl_oglfont$(DependSuffix)
	$(RM) $(IntermediateDirectory)/opengl_oglfont$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/opengl_opengl$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/opengl_opengl$(DependSuffix)
	$(RM) $(IntermediateDirectory)/opengl_opengl$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/opengl_openglext$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/opengl_openglext$(DependSuffix)
	$(RM) $(IntermediateDirectory)/opengl_openglext$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/opengl_shader$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/opengl_shader$(DependSuffix)
	$(RM) $(IntermediateDirectory)/opengl_shader$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/opengl_texture$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/opengl_texture$(DependSuffix)
	$(RM) $(IntermediateDirectory)/opengl_texture$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/opengl_window$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/opengl_window$(DependSuffix)
	$(RM) $(IntermediateDirectory)/opengl_window$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/opengl_fbo$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/opengl_fbo$(DependSuffix)
	$(RM) $(IntermediateDirectory)/opengl_fbo$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/opengl_vao$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/opengl_vao$(DependSuffix)
	$(RM) $(IntermediateDirectory)/opengl_vao$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/sound_sound$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/sound_sound$(DependSuffix)
	$(RM) $(IntermediateDirectory)/sound_sound$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/sound_stream_channel$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/sound_stream_channel$(DependSuffix)
	$(RM) $(IntermediateDirectory)/sound_stream_channel$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/sound_stream$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/sound_stream$(DependSuffix)
	$(RM) $(IntermediateDirectory)/sound_stream$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/threading_thread_controller$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/threading_thread_controller$(DependSuffix)
	$(RM) $(IntermediateDirectory)/threading_thread_controller$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/threading_thread_worker$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/threading_thread_worker$(DependSuffix)
	$(RM) $(IntermediateDirectory)/threading_thread_worker$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/threading_thread_pool$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/threading_thread_pool$(DependSuffix)
	$(RM) $(IntermediateDirectory)/threading_thread_pool$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/timing_highperf$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/timing_highperf$(DependSuffix)
	$(RM) $(IntermediateDirectory)/timing_highperf$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/timing_timer$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/timing_timer$(DependSuffix)
	$(RM) $(IntermediateDirectory)/timing_timer$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/voxels_voxelizer$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/voxels_voxelizer$(DependSuffix)
	$(RM) $(IntermediateDirectory)/voxels_voxelizer$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/voxels_voxelmodel$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/voxels_voxelmodel$(DependSuffix)
	$(RM) $(IntermediateDirectory)/voxels_voxelmodel$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) $(OutputFile).exe
	$(RM) ".build-debug/fbcraft"


