##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=fbcraft
ConfigurationName      :=Debug
WorkspacePath          := "E:\Dropbox\Dropbox\fbcraft"
ProjectPath            := "E:\Dropbox\Dropbox\fbcraft"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=GONZO
Date                   :=28.06.2013
CodeLitePath           :="E:\CodeLite"
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
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)inc 
IncludePCH             := 
RcIncludePath          := 
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
CodeLiteDir:=E:\CodeLite
Objects0=$(IntermediateDirectory)/src_blocks$(ObjectSuffix) $(IntermediateDirectory)/src_gameman$(ObjectSuffix) $(IntermediateDirectory)/src_input$(ObjectSuffix) $(IntermediateDirectory)/src_main$(ObjectSuffix) $(IntermediateDirectory)/src_renderman$(ObjectSuffix) $(IntermediateDirectory)/src_seamless$(ObjectSuffix) $(IntermediateDirectory)/src_shaderman$(ObjectSuffix) $(IntermediateDirectory)/src_soundman$(ObjectSuffix) $(IntermediateDirectory)/src_textureman$(ObjectSuffix) $(IntermediateDirectory)/fbcraft.rc$(ObjectSuffix) \
	$(IntermediateDirectory)/engine_bitmap$(ObjectSuffix) $(IntermediateDirectory)/engine_config$(ObjectSuffix) $(IntermediateDirectory)/engine_frustum$(ObjectSuffix) $(IntermediateDirectory)/engine_log$(ObjectSuffix) $(IntermediateDirectory)/engine_matrix$(ObjectSuffix) $(IntermediateDirectory)/engine_music$(ObjectSuffix) $(IntermediateDirectory)/engine_opengl$(ObjectSuffix) $(IntermediateDirectory)/engine_openglext$(ObjectSuffix) $(IntermediateDirectory)/engine_shader$(ObjectSuffix) $(IntermediateDirectory)/engine_sound$(ObjectSuffix) \
	$(IntermediateDirectory)/engine_texture$(ObjectSuffix) $(IntermediateDirectory)/engine_vector$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_blocks$(ObjectSuffix): src/blocks.cpp $(IntermediateDirectory)/src_blocks$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/blocks.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_blocks$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_blocks$(DependSuffix): src/blocks.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_blocks$(ObjectSuffix) -MF$(IntermediateDirectory)/src_blocks$(DependSuffix) -MM "src/blocks.cpp"

$(IntermediateDirectory)/src_blocks$(PreprocessSuffix): src/blocks.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_blocks$(PreprocessSuffix) "src/blocks.cpp"

$(IntermediateDirectory)/src_gameman$(ObjectSuffix): src/gameman.cpp $(IntermediateDirectory)/src_gameman$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/gameman.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_gameman$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_gameman$(DependSuffix): src/gameman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_gameman$(ObjectSuffix) -MF$(IntermediateDirectory)/src_gameman$(DependSuffix) -MM "src/gameman.cpp"

$(IntermediateDirectory)/src_gameman$(PreprocessSuffix): src/gameman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_gameman$(PreprocessSuffix) "src/gameman.cpp"

$(IntermediateDirectory)/src_input$(ObjectSuffix): src/input.cpp $(IntermediateDirectory)/src_input$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/input.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_input$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_input$(DependSuffix): src/input.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_input$(ObjectSuffix) -MF$(IntermediateDirectory)/src_input$(DependSuffix) -MM "src/input.cpp"

$(IntermediateDirectory)/src_input$(PreprocessSuffix): src/input.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_input$(PreprocessSuffix) "src/input.cpp"

$(IntermediateDirectory)/src_main$(ObjectSuffix): src/main.cpp $(IntermediateDirectory)/src_main$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main$(DependSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main$(DependSuffix) -MM "src/main.cpp"

$(IntermediateDirectory)/src_main$(PreprocessSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main$(PreprocessSuffix) "src/main.cpp"

$(IntermediateDirectory)/src_renderman$(ObjectSuffix): src/renderman.cpp $(IntermediateDirectory)/src_renderman$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/renderman.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_renderman$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_renderman$(DependSuffix): src/renderman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_renderman$(ObjectSuffix) -MF$(IntermediateDirectory)/src_renderman$(DependSuffix) -MM "src/renderman.cpp"

$(IntermediateDirectory)/src_renderman$(PreprocessSuffix): src/renderman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_renderman$(PreprocessSuffix) "src/renderman.cpp"

$(IntermediateDirectory)/src_seamless$(ObjectSuffix): src/seamless.cpp $(IntermediateDirectory)/src_seamless$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/seamless.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_seamless$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_seamless$(DependSuffix): src/seamless.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_seamless$(ObjectSuffix) -MF$(IntermediateDirectory)/src_seamless$(DependSuffix) -MM "src/seamless.cpp"

$(IntermediateDirectory)/src_seamless$(PreprocessSuffix): src/seamless.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_seamless$(PreprocessSuffix) "src/seamless.cpp"

$(IntermediateDirectory)/src_shaderman$(ObjectSuffix): src/shaderman.cpp $(IntermediateDirectory)/src_shaderman$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/shaderman.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_shaderman$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_shaderman$(DependSuffix): src/shaderman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_shaderman$(ObjectSuffix) -MF$(IntermediateDirectory)/src_shaderman$(DependSuffix) -MM "src/shaderman.cpp"

$(IntermediateDirectory)/src_shaderman$(PreprocessSuffix): src/shaderman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_shaderman$(PreprocessSuffix) "src/shaderman.cpp"

$(IntermediateDirectory)/src_soundman$(ObjectSuffix): src/soundman.cpp $(IntermediateDirectory)/src_soundman$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/soundman.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_soundman$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_soundman$(DependSuffix): src/soundman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_soundman$(ObjectSuffix) -MF$(IntermediateDirectory)/src_soundman$(DependSuffix) -MM "src/soundman.cpp"

$(IntermediateDirectory)/src_soundman$(PreprocessSuffix): src/soundman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_soundman$(PreprocessSuffix) "src/soundman.cpp"

$(IntermediateDirectory)/src_textureman$(ObjectSuffix): src/textureman.cpp $(IntermediateDirectory)/src_textureman$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/textureman.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_textureman$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_textureman$(DependSuffix): src/textureman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_textureman$(ObjectSuffix) -MF$(IntermediateDirectory)/src_textureman$(DependSuffix) -MM "src/textureman.cpp"

$(IntermediateDirectory)/src_textureman$(PreprocessSuffix): src/textureman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_textureman$(PreprocessSuffix) "src/textureman.cpp"

$(IntermediateDirectory)/fbcraft.rc$(ObjectSuffix): fbcraft.rc
	$(RcCompilerName) -i "E:/Dropbox/Dropbox/fbcraft/fbcraft.rc" $(RcCmpOptions)   $(ObjectSwitch)$(IntermediateDirectory)/fbcraft.rc$(ObjectSuffix) $(RcIncludePath)
$(IntermediateDirectory)/engine_bitmap$(ObjectSuffix): src/engine/bitmap.cpp $(IntermediateDirectory)/engine_bitmap$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/engine/bitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/engine_bitmap$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/engine_bitmap$(DependSuffix): src/engine/bitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/engine_bitmap$(ObjectSuffix) -MF$(IntermediateDirectory)/engine_bitmap$(DependSuffix) -MM "src/engine/bitmap.cpp"

$(IntermediateDirectory)/engine_bitmap$(PreprocessSuffix): src/engine/bitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/engine_bitmap$(PreprocessSuffix) "src/engine/bitmap.cpp"

$(IntermediateDirectory)/engine_config$(ObjectSuffix): src/engine/config.cpp $(IntermediateDirectory)/engine_config$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/engine/config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/engine_config$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/engine_config$(DependSuffix): src/engine/config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/engine_config$(ObjectSuffix) -MF$(IntermediateDirectory)/engine_config$(DependSuffix) -MM "src/engine/config.cpp"

$(IntermediateDirectory)/engine_config$(PreprocessSuffix): src/engine/config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/engine_config$(PreprocessSuffix) "src/engine/config.cpp"

$(IntermediateDirectory)/engine_frustum$(ObjectSuffix): src/engine/frustum.cpp $(IntermediateDirectory)/engine_frustum$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/engine/frustum.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/engine_frustum$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/engine_frustum$(DependSuffix): src/engine/frustum.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/engine_frustum$(ObjectSuffix) -MF$(IntermediateDirectory)/engine_frustum$(DependSuffix) -MM "src/engine/frustum.cpp"

$(IntermediateDirectory)/engine_frustum$(PreprocessSuffix): src/engine/frustum.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/engine_frustum$(PreprocessSuffix) "src/engine/frustum.cpp"

$(IntermediateDirectory)/engine_log$(ObjectSuffix): src/engine/log.cpp $(IntermediateDirectory)/engine_log$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/engine/log.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/engine_log$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/engine_log$(DependSuffix): src/engine/log.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/engine_log$(ObjectSuffix) -MF$(IntermediateDirectory)/engine_log$(DependSuffix) -MM "src/engine/log.cpp"

$(IntermediateDirectory)/engine_log$(PreprocessSuffix): src/engine/log.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/engine_log$(PreprocessSuffix) "src/engine/log.cpp"

$(IntermediateDirectory)/engine_matrix$(ObjectSuffix): src/engine/matrix.cpp $(IntermediateDirectory)/engine_matrix$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/engine/matrix.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/engine_matrix$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/engine_matrix$(DependSuffix): src/engine/matrix.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/engine_matrix$(ObjectSuffix) -MF$(IntermediateDirectory)/engine_matrix$(DependSuffix) -MM "src/engine/matrix.cpp"

$(IntermediateDirectory)/engine_matrix$(PreprocessSuffix): src/engine/matrix.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/engine_matrix$(PreprocessSuffix) "src/engine/matrix.cpp"

$(IntermediateDirectory)/engine_music$(ObjectSuffix): src/engine/music.cpp $(IntermediateDirectory)/engine_music$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/engine/music.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/engine_music$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/engine_music$(DependSuffix): src/engine/music.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/engine_music$(ObjectSuffix) -MF$(IntermediateDirectory)/engine_music$(DependSuffix) -MM "src/engine/music.cpp"

$(IntermediateDirectory)/engine_music$(PreprocessSuffix): src/engine/music.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/engine_music$(PreprocessSuffix) "src/engine/music.cpp"

$(IntermediateDirectory)/engine_opengl$(ObjectSuffix): src/engine/opengl.cpp $(IntermediateDirectory)/engine_opengl$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/engine/opengl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/engine_opengl$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/engine_opengl$(DependSuffix): src/engine/opengl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/engine_opengl$(ObjectSuffix) -MF$(IntermediateDirectory)/engine_opengl$(DependSuffix) -MM "src/engine/opengl.cpp"

$(IntermediateDirectory)/engine_opengl$(PreprocessSuffix): src/engine/opengl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/engine_opengl$(PreprocessSuffix) "src/engine/opengl.cpp"

$(IntermediateDirectory)/engine_openglext$(ObjectSuffix): src/engine/openglext.cpp $(IntermediateDirectory)/engine_openglext$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/engine/openglext.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/engine_openglext$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/engine_openglext$(DependSuffix): src/engine/openglext.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/engine_openglext$(ObjectSuffix) -MF$(IntermediateDirectory)/engine_openglext$(DependSuffix) -MM "src/engine/openglext.cpp"

$(IntermediateDirectory)/engine_openglext$(PreprocessSuffix): src/engine/openglext.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/engine_openglext$(PreprocessSuffix) "src/engine/openglext.cpp"

$(IntermediateDirectory)/engine_shader$(ObjectSuffix): src/engine/shader.cpp $(IntermediateDirectory)/engine_shader$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/engine/shader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/engine_shader$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/engine_shader$(DependSuffix): src/engine/shader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/engine_shader$(ObjectSuffix) -MF$(IntermediateDirectory)/engine_shader$(DependSuffix) -MM "src/engine/shader.cpp"

$(IntermediateDirectory)/engine_shader$(PreprocessSuffix): src/engine/shader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/engine_shader$(PreprocessSuffix) "src/engine/shader.cpp"

$(IntermediateDirectory)/engine_sound$(ObjectSuffix): src/engine/sound.cpp $(IntermediateDirectory)/engine_sound$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/engine/sound.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/engine_sound$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/engine_sound$(DependSuffix): src/engine/sound.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/engine_sound$(ObjectSuffix) -MF$(IntermediateDirectory)/engine_sound$(DependSuffix) -MM "src/engine/sound.cpp"

$(IntermediateDirectory)/engine_sound$(PreprocessSuffix): src/engine/sound.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/engine_sound$(PreprocessSuffix) "src/engine/sound.cpp"

$(IntermediateDirectory)/engine_texture$(ObjectSuffix): src/engine/texture.cpp $(IntermediateDirectory)/engine_texture$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/engine/texture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/engine_texture$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/engine_texture$(DependSuffix): src/engine/texture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/engine_texture$(ObjectSuffix) -MF$(IntermediateDirectory)/engine_texture$(DependSuffix) -MM "src/engine/texture.cpp"

$(IntermediateDirectory)/engine_texture$(PreprocessSuffix): src/engine/texture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/engine_texture$(PreprocessSuffix) "src/engine/texture.cpp"

$(IntermediateDirectory)/engine_vector$(ObjectSuffix): src/engine/vector.cpp $(IntermediateDirectory)/engine_vector$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "E:/Dropbox/Dropbox/fbcraft/src/engine/vector.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/engine_vector$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/engine_vector$(DependSuffix): src/engine/vector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/engine_vector$(ObjectSuffix) -MF$(IntermediateDirectory)/engine_vector$(DependSuffix) -MM "src/engine/vector.cpp"

$(IntermediateDirectory)/engine_vector$(PreprocessSuffix): src/engine/vector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/engine_vector$(PreprocessSuffix) "src/engine/vector.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/src_blocks$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_blocks$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_blocks$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_gameman$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_gameman$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_gameman$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_input$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_input$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_input$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_renderman$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_renderman$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_renderman$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_seamless$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_seamless$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_seamless$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_shaderman$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_shaderman$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_shaderman$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_soundman$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_soundman$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_soundman$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_textureman$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_textureman$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_textureman$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/fbcraft.rc$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/engine_bitmap$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/engine_bitmap$(DependSuffix)
	$(RM) $(IntermediateDirectory)/engine_bitmap$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/engine_config$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/engine_config$(DependSuffix)
	$(RM) $(IntermediateDirectory)/engine_config$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/engine_frustum$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/engine_frustum$(DependSuffix)
	$(RM) $(IntermediateDirectory)/engine_frustum$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/engine_log$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/engine_log$(DependSuffix)
	$(RM) $(IntermediateDirectory)/engine_log$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/engine_matrix$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/engine_matrix$(DependSuffix)
	$(RM) $(IntermediateDirectory)/engine_matrix$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/engine_music$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/engine_music$(DependSuffix)
	$(RM) $(IntermediateDirectory)/engine_music$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/engine_opengl$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/engine_opengl$(DependSuffix)
	$(RM) $(IntermediateDirectory)/engine_opengl$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/engine_openglext$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/engine_openglext$(DependSuffix)
	$(RM) $(IntermediateDirectory)/engine_openglext$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/engine_shader$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/engine_shader$(DependSuffix)
	$(RM) $(IntermediateDirectory)/engine_shader$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/engine_sound$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/engine_sound$(DependSuffix)
	$(RM) $(IntermediateDirectory)/engine_sound$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/engine_texture$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/engine_texture$(DependSuffix)
	$(RM) $(IntermediateDirectory)/engine_texture$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/engine_vector$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/engine_vector$(DependSuffix)
	$(RM) $(IntermediateDirectory)/engine_vector$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) $(OutputFile).exe
	$(RM) ".build-debug/fbcraft"


