format_version = "1.0"

local maxArraySize = 256
local maxArrayValue = math.pow(2, 16) -- bits 0-7 -> min (or avg), bits 8-15 -> max
local maxHistorySize = 117 -- max # of LCD pages (for highest sampling rate, 10s history at 192k)

local propArrayStartTag = 2000

local guiOwnerProperties = {}
local documentOwnerProperties = {}
local rtOwnerProperties = {}

for i = 1, maxArraySize do
  -- range is [0-100] for regular values. 200 is clipping below, 300 is clipping above
  rtOwnerProperties["prop_array_" .. i] = jbox.number {
    property_tag = propArrayStartTag + i,
    default = 50, -- which means 0 / middle of screen
    steps = maxArrayValue,
    ui_name = jbox.ui_text("propertyname prop_array_" .. i),
    ui_type = jbox.ui_linear { min = 0.0, max = 1.0 }
  }
end

rtOwnerProperties["prop_array_start"] = jbox.number {
  property_tag = 3000,
  steps = maxArraySize,
  default = 0,
  ui_name = jbox.ui_text("propertyname prop_array_start"),
  ui_type = jbox.ui_linear { min = 0.0, max = 1.0 }
}

-- scale for display 0 = 1x, 1 = 100x, 2 = 127x
documentOwnerProperties["prop_cv_in_1_display_scale"] = jbox.number {
  property_tag = 3150,
  default = 2, -- 1x
  steps = 3,
  ui_name = jbox.ui_text("propertyname prop_cv_in_1_display_scale"),
  ui_type = jbox.ui_selector({
    jbox.ui_text("ui_selector prop_cv_in_1_display_scale 1x"),
    jbox.ui_text("ui_selector prop_cv_in_1_display_scale 100x"),
    jbox.ui_text("ui_selector prop_cv_in_1_display_scale 127x"),
  })
}

-- x * 100000 + 100000 to be in range [0 - 200000] (5 digit precision) when no int part (includes sign)
-- x * 100000 when int part (absolute value)
-- 200001 means ignore
rtOwnerProperties["prop_cv_in_1_value"] = jbox.number {
  property_tag = 3200,
  default = 200001,
  steps = 200002,
  ui_name = jbox.ui_text("propertyname prop_cv_in_1_value"),
  ui_type = jbox.ui_linear { min = -1.0, max = 1.0, units={{decimals=4}} }
}

-- represents the integer part of cv_in_1_value
rtOwnerProperties["prop_cv_in_1_value_int"] = jbox.number {
  property_tag = 3201,
  default = 0,
  steps = 20002,
  ui_name = jbox.ui_text("propertyname prop_cv_in_1_value_int"),
  ui_type = jbox.ui_linear { min = 0, max = 10000, units={{decimals=4}} }
}

-- each bit represent a state => bit 0 is connected/not connected
rtOwnerProperties["prop_cv_in_1_state"] = jbox.number {
  property_tag = 3300,
  steps = 2,
  default = 0,
  ui_name = jbox.ui_text("propertyname prop_cv_in_1_state"),
  ui_type = jbox.ui_linear { min = -1.0, max = 1.0, units={{decimals=4}} }
}

-- note uses the formula clamp<TJBox_Int32>(value * 127.f + 0.1f, 0, 127)
rtOwnerProperties["prop_cv_in_1_value_as_note"] = jbox.number {
  property_tag = 3400,
  steps = 128,
  default = 0,
  ui_name = jbox.ui_text("propertyname prop_cv_in_1_value_as_note"),
  ui_type = jbox.ui_linear { min = 0.0, max = 1.0 }
}

-- x * 100000 + 100000 to be in range [0 - 200000] (5 digit precision) when no int part (includes sign)
-- x * 100000 when int part (absolute value)
-- 200001 means ignore
rtOwnerProperties["prop_cv_in_1_min_value"] = jbox.number {
  property_tag = 3500,
  default = 200001,
  steps = 200002,
  ui_name = jbox.ui_text("propertyname prop_cv_in_1_min_value"),
  ui_type = jbox.ui_linear { min = -1.0, max = 1.0, units={{decimals=4}} }
}

-- represents the integer part of prop_cv_in_1_min_value
rtOwnerProperties["prop_cv_in_1_min_value_int"] = jbox.number {
  property_tag = 3501,
  default = 0,
  steps = 20002,
  ui_name = jbox.ui_text("propertyname prop_cv_in_1_min_value_int"),
  ui_type = jbox.ui_linear { min = 0, max = 10000, units={{decimals=4}} }
}

-- x * 100000 + 100000 to be in range [0 - 200000] (5 digit precision) when no int part (includes sign)
-- x * 100000 when int part (absolute value)
-- 200001 means ignore
rtOwnerProperties["prop_cv_in_1_max_value"] = jbox.number {
  property_tag = 3600,
  default = 200001,
  steps = 200002,
  ui_name = jbox.ui_text("propertyname prop_cv_in_1_max_value"),
  ui_type = jbox.ui_linear { min = -1.0, max = 1.0, units={{decimals=4}} }
}

-- represents the integer part of prop_cv_in_1_max_value
rtOwnerProperties["prop_cv_in_1_max_value_int"] = jbox.number {
  property_tag = 3601,
  default = 0,
  steps = 20002,
  ui_name = jbox.ui_text("propertyname prop_cv_in_1_max_value_int"),
  ui_type = jbox.ui_linear { min = 0, max = 10000, units={{decimals=4}} }
}

documentOwnerProperties["prop_cv_in_1_min_max_reset"] = jbox.boolean {
  property_tag = 3700,
  default = false,
  ui_name = jbox.ui_text("propertyname prop_cv_in_1_min_max_reset"),
  ui_type = jbox.ui_selector({
    jbox.UI_TEXT_OFF,
    jbox.UI_TEXT_ON,
  }),
  persistence = "none"
}

-- whether the input is paused or not
documentOwnerProperties["prop_input_paused"] = jbox.boolean {
  property_tag = 3900,
  default = false,
  ui_name = jbox.ui_text("propertyname prop_input_paused"),
  ui_type = jbox.ui_selector({
    jbox.UI_TEXT_ON,
    jbox.UI_TEXT_OFF,
  }),
  persistence = "none"
}

-- used in pause mode to indicate which value is currently shown in the page
documentOwnerProperties["prop_input_page_offset"] = jbox.number {
  property_tag = 4100,
  steps = maxArraySize,
  default = maxArraySize - 1,
  ui_name = jbox.ui_text("propertyname prop_input_page_offset"),
  ui_type = jbox.ui_linear { min = 0, max = maxArraySize - 1 },
  persistence = "none"
}

-- used in pause mode to indicate where we are in the history
-- floating point number represented as an integer (between 0 and 10000)
documentOwnerProperties["prop_input_history_offset"] = jbox.number {
  property_tag = 4200,
  steps = 10001,
  default = 10000,
  ui_name = jbox.ui_text("propertyname prop_input_history_offset"),
  ui_type = jbox.ui_linear { min = 0, max = 10000 },
  persistence = "none"
}

-- used in pause mode to indicate where we are in the history (override from rt!)
-- 10001 means no override
rtOwnerProperties["prop_input_history_offset_rt"] = jbox.number {
  property_tag = 4300,
  steps = 10002,
  default = 10001,
  ui_name = jbox.ui_text("propertyname prop_input_history_offset_rt"),
  ui_type = jbox.ui_linear { min = 0.0, max = 1.0 },
  persistence = "none"
}

-- Zoom Factor (X-axis)
documentOwnerProperties["prop_zoom_factor_x"] = jbox.number {
  property_tag = 4400,
  default = 1.0,
  ui_name = jbox.ui_text("propertyname prop_zoom_factor_x"),
  ui_type = jbox.ui_linear { min = 1.0, max = 100.0, units={{decimals=0}} },
}

-- Zoom Factor (Y-axis)
documentOwnerProperties["prop_zoom_factor_y"] = jbox.number {
  property_tag = 4450,
  default = 1.0,
  ui_name = jbox.ui_text("propertyname prop_zoom_factor_y"),
  ui_type = jbox.ui_linear { min = 0.01, max = 1.0, units={{decimals=2}} },
}

-- location of +0.5 axis (depends on prop_zoom_factor_y)
rtOwnerProperties["prop_plus_05_axis"] = jbox.number {
  property_tag = 4475,
  steps = maxArrayValue,
  default = 0,
  ui_name = jbox.ui_text("N/A"),
  ui_type = jbox.ui_linear { min = 0.0, max = 1.0 }
}

-- whether min/max auto reset. 0 means no auto reset. 1-10 represents how many seconds
documentOwnerProperties["prop_cv_in_1_min_max_auto_reset"] = jbox.number {
  property_tag = 4500,
  steps = 11,
  default = 0,
  ui_name = jbox.ui_text("propertyname prop_cv_in_1_min_max_auto_reset"),
  ui_type = jbox.ui_linear { min = 0, max = 10,
    units = {
      min_text = jbox.UI_TEXT_OFF,
      {
        decimals = 0,
        min_value = 0,
        unit = { template = jbox.ui_text("ui_selector prop_cv_in_1_min_max_auto_reset"), base = 1 }
      }
    }
  },
}

-- blue/green/red/yellow/white
documentOwnerProperties["prop_cv_in_1_color"] = jbox.number {
  property_tag = 4600,
  default = 0, -- blue
  steps = 5,
  ui_name = jbox.ui_text("propertyname prop_cv_in_1_color"),
  ui_type = jbox.ui_selector({
    jbox.ui_text("ui_selector prop_cv_in_1_color 0"),
    jbox.ui_text("ui_selector prop_cv_in_1_color 1"),
    jbox.ui_text("ui_selector prop_cv_in_1_color 2"),
    jbox.ui_text("ui_selector prop_cv_in_1_color 3"),
    jbox.ui_text("ui_selector prop_cv_in_1_color 4"),
  })
}

-- cv out type: 0=passthrough, 1=inverse, 2=unipolar, 3=bipolar
documentOwnerProperties["prop_cv_out_1_type"] = jbox.number {
  property_tag = 4700,
  default = 0, -- passthrough
  steps = 4,
  ui_name = jbox.ui_text("propertyname prop_cv_out_1_type"),
  ui_type = jbox.ui_selector({
    jbox.ui_text("ui_selector prop_cv_out_type passthrough"),
    jbox.ui_text("ui_selector prop_cv_out_type inverse"),
    jbox.ui_text("ui_selector prop_cv_out_type unipolar"),
    jbox.ui_text("ui_selector prop_cv_out_type bipolar"),
  })
}

-- cv out type: 0=passthrough, 1=inverse, 2=unipolar, 3=bipolar
documentOwnerProperties["prop_cv_out_2_type"] = jbox.number {
  property_tag = 4701,
  default = 0, -- passthrough
  steps = 4,
  ui_name = jbox.ui_text("propertyname prop_cv_out_2_type"),
  ui_type = jbox.ui_selector({
    jbox.ui_text("ui_selector prop_cv_out_type passthrough"),
    jbox.ui_text("ui_selector prop_cv_out_type inverse"),
    jbox.ui_text("ui_selector prop_cv_out_type unipolar"),
    jbox.ui_text("ui_selector prop_cv_out_type bipolar"),
  })
}

-- cv out type: 0=passthrough, 1=inverse, 2=unipolar, 3=bipolar
documentOwnerProperties["prop_cv_out_3_type"] = jbox.number {
  property_tag = 4702,
  default = 0, -- passthrough
  steps = 4,
  ui_name = jbox.ui_text("propertyname prop_cv_out_3_type"),
  ui_type = jbox.ui_selector({
    jbox.ui_text("ui_selector prop_cv_out_type passthrough"),
    jbox.ui_text("ui_selector prop_cv_out_type inverse"),
    jbox.ui_text("ui_selector prop_cv_out_type unipolar"),
    jbox.ui_text("ui_selector prop_cv_out_type bipolar"),
  })
}

-- cv out type: 0=passthrough, 1=inverse, 2=unipolar, 3=bipolar
documentOwnerProperties["prop_cv_out_4_type"] = jbox.number {
  property_tag = 4703,
  default = 0, -- passthrough
  steps = 4,
  ui_name = jbox.ui_text("propertyname prop_cv_out_4_type"),
  ui_type = jbox.ui_selector({
    jbox.ui_text("ui_selector prop_cv_out_type passthrough"),
    jbox.ui_text("ui_selector prop_cv_out_type inverse"),
    jbox.ui_text("ui_selector prop_cv_out_type unipolar"),
    jbox.ui_text("ui_selector prop_cv_out_type bipolar"),
  })
}

-- whether the screen is on or off (graph portion of the screen, not keyboard)
documentOwnerProperties["prop_screen_on"] = jbox.boolean {
  property_tag = 4800,
  default = true,
  ui_name = jbox.ui_text("propertyname prop_screen_on"),
  ui_type = jbox.ui_selector({
    jbox.UI_TEXT_OFF,
    jbox.UI_TEXT_ON,
  }),
}

-- whether the midi section is on or off (keyboard + note + gate)
documentOwnerProperties["prop_midi_on"] = jbox.boolean {
  property_tag = 4900,
  default = true,
  ui_name = jbox.ui_text("propertyname prop_midi_on"),
  ui_type = jbox.ui_selector({
    jbox.UI_TEXT_OFF,
    jbox.UI_TEXT_ON,
  }),
}

--[[
Custom properties
--]]
custom_properties = jbox.property_set {
  gui_owner = {
    properties = guiOwnerProperties
  },

  document_owner = {
    properties = documentOwnerProperties
  },

  rtc_owner = {
    properties = {
      instance = jbox.native_object{ },
    }
  },

  rt_owner = {
    properties = rtOwnerProperties
  }
}

--[[
Inputs/Outputs
--]]
cv_inputs = {}
cv_outputs = {}

cv_inputs["cv_in_1"] = jbox.cv_input {
  ui_name = jbox.ui_text("propertyname cv_in_1")
}

cv_outputs["cv_out_1"] = jbox.cv_output {
  ui_name = jbox.ui_text("propertyname cv_out_1")
}

cv_outputs["cv_out_2"] = jbox.cv_output {
  ui_name = jbox.ui_text("propertyname cv_out_2")
}

cv_outputs["cv_out_3"] = jbox.cv_output {
  ui_name = jbox.ui_text("propertyname cv_out_3")
}

cv_outputs["cv_out_4"] = jbox.cv_output {
  ui_name = jbox.ui_text("propertyname cv_out_4")
}

cv_outputs["cv_out_value_1"] = jbox.cv_output {
  ui_name = jbox.ui_text("propertyname cv_out_value_1")
}

cv_outputs["cv_out_min_value_1"] = jbox.cv_output {
  ui_name = jbox.ui_text("propertyname cv_out_min_value_1")
}

cv_outputs["cv_out_max_value_1"] = jbox.cv_output {
  ui_name = jbox.ui_text("propertyname cv_out_max_value_1")
}

-- allow for automation
midi_implementation_chart = {
  midi_cc_chart = {
    [12] = "/custom_properties/prop_input_page_offset",
    [13] = "/custom_properties/prop_input_history_offset",
    [14] = "/custom_properties/prop_input_paused",
    [15] = "/custom_properties/prop_zoom_factor_x",
    [16] = "/custom_properties/prop_cv_in_1_min_max_reset",
    [17] = "/custom_properties/prop_cv_in_1_display_scale",
    [18] = "/custom_properties/prop_cv_in_1_min_max_auto_reset",
    [19] = "/custom_properties/prop_cv_in_1_color",
  }
}

-- allow for hardware
remote_implementation_chart = {
  ["/custom_properties/prop_input_page_offset"] = {
    internal_name = "Graph Select",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_input_page_offset"),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_input_page_offset")
  },

  ["/custom_properties/prop_input_history_offset"] = {
    internal_name = "Graph Scroll",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_input_history_offset"),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_input_history_offset")
  },

  ["/custom_properties/prop_input_paused"] = {
    internal_name = "Live View",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_input_paused"),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_input_paused")
  },

  ["/custom_properties/prop_zoom_factor_x"] = {
    internal_name = "Zoom",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_zoom_factor_x"),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_zoom_factor_x")
  },

  ["/custom_properties/prop_zoom_factor_y"] = {
    internal_name = "Scale",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_zoom_factor_y"),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_zoom_factor_y")
  },

  ["/custom_properties/prop_cv_in_1_min_max_reset"] = {
    internal_name = "Min Max Reset",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_cv_in_1_min_max_reset"),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_cv_in_1_min_max_reset")
  },

  ["/custom_properties/prop_cv_in_1_display_scale"] = {
    internal_name = "Numeric Scale",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_cv_in_1_display_scale"),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_cv_in_1_display_scale")
  },

  ["/custom_properties/prop_cv_in_1_min_max_auto_reset"] = {
    internal_name = "Min Max Auto Reset",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_cv_in_1_min_max_auto_reset"),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_cv_in_1_min_max_auto_reset")
  },

  ["/custom_properties/prop_cv_in_1_color"] = {
    internal_name = "Color",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_cv_in_1_color"),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_cv_in_1_color")
  },

  ["/custom_properties/prop_screen_on"] = {
    internal_name = "Graph",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_screen_on"),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_screen_on")
  },

  ["/custom_properties/prop_midi_on"] = {
    internal_name = "Keyboard",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_midi_on"),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_midi_on")
  },
}
