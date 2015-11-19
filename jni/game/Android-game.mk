LOCAL_PATH   := $(PATH1)
APP_STL:=stlport_static gnustl_static

#include $(CLEAR_VARS)
#LOCAL_MODULE := bullet 
#LOCAL_SRC_FILES := libbullet.so 
#include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := devil
LOCAL_SRC_FILES := libdevil.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_PATH   := $(PATH1)/game
LOCAL_MODULE    := game
fDPath :=fileDeal
LOCAL_SHARED_LIBRARIES :=devil #bullet 
# for native asset manager
LOCAL_LDLIBS    += -landroid

#LOCAL_LIBS := $(LOCAL_PATH)/libiconv.so $(LOCAL_PATH)/libxml2.so\
$(LOCAL_PATH)/libbullet.so  
                                                          					
#LOCAL_C_INCLUDES := jni/game/fileDeal\
                    $(ENGINE_PATH)/bullet\
                    $(ENGINE_PATH)/bullet/BulletCollision\
                    $(ENGINE_PATH)/bullet/BulletDynamics\
                    $(ENGINE_PATH)/bullet/BulletMultiThreaded\
                    $(ENGINE_PATH)/bullet/BulletSoftBody\
                    $(ENGINE_PATH)/bullet/LinearMath\
                    $(ENGINE_PATH)/bullet/MiniCL
                    
#LOCAL_PATH_CONV:= $(PATH1)/game/libiconv-1.14                    
#LOCAL_C_INCLUDES += \
  $(LOCAL_PATH_CONV)/include \
  $(LOCAL_PATH_CONV)/libcharset \
  $(LOCAL_PATH_CONV)/lib \
  $(LOCAL_PATH_CONV)/libcharset/include \
  $(LOCAL_PATH_CONV)/srclib
   
#LOCAL_PATH_XML2:= $(PATH1)/game/libxml2-2.8.0
#LOCAL_C_INCLUDES += \
$(LOCAL_PATH_XML2)/include \
$(LOCAL_PATH_XML2)/include/libxml \
$(LOCAL_PATH_XML2)/../libiconv/include \
$(LOCAL_PATH_XML2)/../libiconv \
$(LOCAL_PATH_XML2)/../libiconv/libcharset \
$(LOCAL_PATH_XML2)/../libiconv/lib \
$(LOCAL_PATH_XML2)/../libiconv/libcharset/include

#physic.cpp\EntityPhysic.cpp\

LOCAL_SRC_FILES :=	$(fDPath)/loadFile.cpp\
					Object.cpp\
                   	Vector3.cpp\
                   	GLSL/TrigFuns.cpp\
                   	GLSL/matrix.cpp\
					GLSL/light.cpp\
					GLSL/material.cpp\
					GLSL/GLSLProj.cpp\
					GLSL/GLSL.cpp\
					GLSL/GLSL_BASE.cpp\
					math/m_math.cpp\
					Line.cpp\
					MOpenil.cpp\
					parser/Parser.cpp\
					parser/ParserLy.cpp\
					Entity.cpp\
					graphic/Frame.cpp\
					graphic/KeyFrame.cpp\
					graphic/animation.cpp\
					graphic/OMatrix.cpp\
					graphic/GraphData.cpp\
					graphic/GraphRace.cpp\
					graphic/GraphRaceGroup.cpp\
					graphic/Transform.cpp\
					graphic/MShape.cpp\
					graphic/Graph.cpp\
					graphic/AnimForGraph.cpp\
					roles/movement.cpp\
					roles/Role.cpp\
					roles/LeadRole.cpp\
					roles/coin.cpp\
					roles/ArmyRole.cpp\
					roles/RoleRace.cpp\
					roles/Thorn.cpp\
					roles/Crystal.cpp\
					roles/Thornball.cpp\
					skyBox.cpp\
					terrain.cpp\
					Camera.cpp\
					MRandom.cpp\
					MHP.cpp\
					LeadHP.cpp\
					PhyPlane.cpp\
					PhyScene.cpp\
					PhySceneQuad.cpp\
					PhySceneCylinder.cpp\
					PhyCylinder.cpp\
					Shadow.cpp\
					GraphShowUp.cpp\
					scene.cpp\
					map.cpp\
					EnemyTeam.cpp\
					EnemyGroup.cpp\
					AppScene.cpp\
					SetScene.cpp\
					SceneTutorial.cpp\
					Square.cpp\
					OptionGraph.cpp\
					MyButton.cpp\
					Screen2d.cpp\
					ShowWord.cpp\
					ShowInfo.cpp\
					SysTime.cpp\
					ETCHeader.cpp\
					MTexture.cpp\
					TextureFactory.cpp\
					RenderEntity.cpp\
					RenderEntityGroup.cpp\
					RenderScene.cpp\
					RenderManager.cpp\
					MJNI.cpp\
					commond.cpp\
					game.cpp\
					gameMain.cpp\
					Movepoint.cpp\
					Movepath.cpp\
					MovepathCircle.cpp\
					GLSL/GLSL_BLOOM.cpp\
					GLSL/GLSL_GAOSI_X.cpp\
					GLSL/GLSL_GAOSI_Y.cpp\
					GLSL/GLSL_getHLight.cpp\
					roles/Bullet0.cpp\
					roles/BulletSmall.cpp\
					BulletSystem.cpp\
                   #physic.cpp\swaper.cpp\
                   Movement.cpp\
                   GData.cpp\
                   AEA.cpp\
                   Entity.cpp\
                   Entitylist.cpp\
                   EntityFactory.cpp\
                   leadRole.cpp\
                   archive.cpp\
                   LObject.cpp\
                   map.cpp\
                   terrain.cpp\
                   skyBox.cpp\
                   Camera.cpp\		
                  # game.cpp\
                 

LOCAL_CPP_FEATURES += exceptions
LOCAL_CPPFLAGS += -fexceptions -frtti
LOCAL_LDLIBS += -llog -lGLESv1_CM -lGLESv2 -lEGL
LOCAL_CFLAGS += -Wno-psabi

include $(BUILD_SHARED_LIBRARY)
