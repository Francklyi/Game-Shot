# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
 
PATH1 :=$(call my-dir)
ENGINE_PATH :=jni/engine
#BULLET_ROOT   :=bullet
#DIR:= $(BULLET_ROOT)
##################################################
#include $(ENGINE_PATH)/Android-bullet.mk
#######################################################################
GAMEDIR :=jni/game
include $(GAMEDIR)/Android-game.mk

#include jni/Android-test.mk
#include jni/$(DIR)/BulletCollision/Android-BulletCollision.mk
# Bullet Physics
##################################################

#######################################################################
