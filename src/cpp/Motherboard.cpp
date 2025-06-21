/*
 * Copyright (c) 2015-2025 pongasoft
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 *
 * @author Yan Pujante
 */

#include "Motherboard.h"
#include <jbox.h>

void MinMaxAutoResetJBoxProperty::onPropertyUpdated(const TJBox_Int32 &, const TJBox_Int32 &autoReset)
{
  if(autoReset == 0)
    fAutoResetFrameCount = MIN_MAX_AUTO_RESET_OFF;
  else
    fAutoResetFrameCount = batchCountToFitDuration(fSampleRate, autoReset * 1000);
}

Motherboard::Motherboard(int iSampleRate):
  fCVIn1("cv_in_1"),
  fCVOut{},
  fCVOutValue1("cv_out_value_1"),
  fCVOutMinValue1("cv_out_min_value_1"),
  fCVOutMaxValue1("cv_out_max_value_1"),
  fPropCVIn1Value("/custom_properties/prop_cv_in_1_value_int", "/custom_properties/prop_cv_in_1_value"),
  fPropCVIn1State("/custom_properties/prop_cv_in_1_state"),
  fPropCVIn1ValueAsNote("/custom_properties/prop_cv_in_1_value_as_note"),
  fPropCVIn1MinValue("/custom_properties/prop_cv_in_1_min_value_int", "/custom_properties/prop_cv_in_1_min_value"),
  fPropCVIn1MaxValue("/custom_properties/prop_cv_in_1_max_value_int", "/custom_properties/prop_cv_in_1_max_value"),
  fPropCVIn1MinMaxReset("/custom_properties/prop_cv_in_1_min_max_reset"),
  fPropInputPaused("/custom_properties/prop_input_paused"),
  fPropInputPageOffset("/custom_properties/prop_input_page_offset"),
  fPropInputHistoryOffset("/custom_properties/prop_input_history_offset"),
  fPropInputHistoryOffsetRT("/custom_properties/prop_input_history_offset_rt"),
  fPropZoomFactorX("/custom_properties/prop_zoom_factor_x"),
  fPropZoomFactorY("/custom_properties/prop_zoom_factor_y"),
  fPropPlus05Axis("/custom_properties/prop_plus_05_axis"),
  fPropCVIn1MinMaxAutoReset("/custom_properties/prop_cv_in_1_min_max_auto_reset", iSampleRate),
  fPropScreenOn("/custom_properties/prop_screen_on"),
  fPropMidiOn("/custom_properties/prop_midi_on"),
  fPropArrayStart("/custom_properties/prop_array_start"),
  fPropArray{}
{
  for(int i = 0; i < MAX_CV_OUT; i++)
    fCVOut[i] = new CVOut(i + 1);

  for(int i = 0; i < MAX_ARRAY_SIZE; i++)
    fPropArray[i] = new Int32JBoxProperty(jbox::PropertyPath::printf("/custom_properties/prop_array_%d", i + 1));
}

void Motherboard::update(Motherboard const &rhs)
{
  fCVIn1 = rhs.fCVIn1;
  for(int i = 0; i < MAX_CV_OUT; i++)
    *fCVOut[i] = *rhs.fCVOut[i];
  fCVOutMinValue1 = rhs.fCVOutMinValue1;
  fCVOutMaxValue1 = rhs.fCVOutMaxValue1;
  fPropCVIn1MinMaxReset = rhs.fPropCVIn1MinMaxReset;
  fPropInputPaused = rhs.fPropInputPaused;
  fPropInputPageOffset = rhs.fPropInputPageOffset;
  fPropInputHistoryOffset = rhs.fPropInputHistoryOffset;
  fPropZoomFactorX = rhs.fPropZoomFactorX;
  fPropZoomFactorY = rhs.fPropZoomFactorY;
  fPropCVIn1MinMaxAutoReset = rhs.fPropCVIn1MinMaxAutoReset;
  fPropScreenOn = rhs.fPropScreenOn;
  fPropMidiOn = rhs.fPropMidiOn;
}

void Motherboard::registerForUpdate(IJBoxPropertyManager &manager)
{
  fPropCVIn1Value.registerForInit(manager);
  fPropCVIn1MinValue.registerForInit(manager);
  fPropCVIn1MaxValue.registerForInit(manager);
  fPropCVIn1ValueAsNote.registerForInit(manager);
  fPropCVIn1State.registerForInit(manager);
  fPropInputHistoryOffsetRT.registerForInit(manager);

  fCVIn1.registerForUpdate(manager);
  for(int i = 0; i < MAX_CV_OUT; i++)
    fCVOut[i]->registerForUpdate(manager, PROP_CV_OUT_1_TYPE_TAG + i);
  fCVOutValue1.registerForUpdate(manager);
  fCVOutMinValue1.registerForUpdate(manager);
  fCVOutMaxValue1.registerForUpdate(manager);
  fPropCVIn1MinMaxReset.registerForUpdate(manager, PROP_CV_IN_1_MIN_MAX_RESET_TAG);
  fPropInputPaused.registerForUpdate(manager, PROP_INPUT_PAUSED_TAG);
  fPropInputPageOffset.registerForUpdate(manager, PROP_INPUT_PAGE_OFFSET_TAG);
  fPropInputHistoryOffset.registerForUpdate(manager, PROP_INPUT_HISTORY_OFFSET_TAG);
  fPropZoomFactorX.registerForUpdate(manager, PROP_ZOOM_FACTOR_X_TAG);
  fPropZoomFactorY.registerForUpdate(manager, PROP_ZOOM_FACTOR_Y_TAG);
  fPropCVIn1MinMaxAutoReset.registerForUpdate(manager, PROP_CV_IN_1_MIN_MAX_AUTO_RESET_TAG);
  fPropScreenOn.registerForUpdate(manager, PROP_SCREEN_ON_TAG);
  fPropMidiOn.registerForUpdate(manager, PROP_MIDI_ON_TAG);
}
