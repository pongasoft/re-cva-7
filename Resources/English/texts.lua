format_version = "1.0"

-- Note that remote name text lenghts are:
--		internal_name = max 64 chars,
--		short_name = max 8 chars,
--		shortest_ui_name = max 4 chars

local maxArraySize = 256

 -- english
texts = {
  ["N/A"] = "N/A",
  ["propertyname cv_in_1"] = "CV In",
  ["propertyname cv_out_value_1"] = "CV",
  ["propertyname cv_out_min_value_1"] = "Min",
  ["propertyname cv_out_max_value_1"] = "Max",
  ["propertyname prop_array_start"] = "N/A",
  ["propertyname prop_cv_in_1_value"] = "N/A",
  ["propertyname prop_cv_in_1_value_int"] = "N/A",
  ["propertyname prop_cv_in_1_state"] = "N/A",
  ["propertyname prop_cv_in_1_value_as_note"] = "N/A",
  ["propertyname prop_cv_in_1_min_value"] = "N/A",
  ["propertyname prop_cv_in_1_min_value_int"] = "N/A",
  ["propertyname prop_cv_in_1_max_value"] = "N/A",
  ["propertyname prop_cv_in_1_max_value_int"] = "N/A",

  ["propertyname prop_cv_in_1_min_max_reset"] = "Reset Min/Max",
  ["ric short_ui_name prop_cv_in_1_min_max_reset"] = "Res. Max",
  ["ric shortest_ui_name prop_cv_in_1_min_max_reset"] = "MMax",

  ["propertyname prop_input_page_offset"] = "Graph Select",
  ["undo prop_input_page_offset"] = "Graph Select",
  ["ric short_ui_name prop_input_page_offset"] = "Gph Sel.",
  ["ric shortest_ui_name prop_input_page_offset"] = "Sel.",

  ["propertyname prop_input_history_offset"] = "Graph Scroll",
  ["undo prop_input_history_offset"] = "Graph Scroll",
  ["ric short_ui_name prop_input_history_offset"] = "Gph Scr.",
  ["ric shortest_ui_name prop_input_history_offset"] = "Scr.",

  ["propertyname prop_input_history_offset_rt"] = "N/A",

  ["propertyname prop_input_paused"] = "Live View",
  ["undo prop_input_paused"] = "Live View",
  ["ric short_ui_name prop_input_paused"] = "LiveView",
  ["ric shortest_ui_name prop_input_paused"] = "LeVw",

  ["propertyname prop_cv_in_1_display_scale"] = "Numeric Scale",
  ["ui_selector prop_cv_in_1_display_scale 1x"] = "Raw",
  ["ui_selector prop_cv_in_1_display_scale 100x"] = "Percent [-100...100]",
  ["ui_selector prop_cv_in_1_display_scale 127x"] = "Integer [-127...127]",
  ["ric short_ui_name prop_cv_in_1_display_scale"] = "NumScale",
  ["ric shortest_ui_name prop_cv_in_1_display_scale"] = "NSca",

  ["propertyname prop_zoom_factor_x"] = "Zoom",
  ["ric short_ui_name prop_zoom_factor_x"] = "Zoom",
  ["ric shortest_ui_name prop_zoom_factor_x"] = "Zoom",

  ["propertyname prop_zoom_factor_y"] = "Scale",
  ["ric short_ui_name prop_zoom_factor_y"] = "Scale",
  ["ric shortest_ui_name prop_zoom_factor_y"] = "Scal",

  ["propertyname prop_cv_in_1_min_max_auto_reset"] = "Min/Max Auto Reset",
  ["ui_selector prop_cv_in_1_min_max_auto_reset"] = "^0s",
  ["ric short_ui_name prop_cv_in_1_min_max_auto_reset"] = "AutoScal",
  ["ric shortest_ui_name prop_cv_in_1_min_max_auto_reset"] = "ASca",

  ["propertyname prop_cv_in_1_color"] = "Color",
  ["ui_selector prop_cv_in_1_color 0"] = "Blue",
  ["ui_selector prop_cv_in_1_color 1"] = "Green",
  ["ui_selector prop_cv_in_1_color 2"] = "Red",
  ["ui_selector prop_cv_in_1_color 3"] = "Yellow",
  ["ui_selector prop_cv_in_1_color 4"] = "White",
  ["ric short_ui_name prop_cv_in_1_color"] = "Color",
  ["ric shortest_ui_name prop_cv_in_1_color"] = "Colr",

  ["propertyname prop_screen_on"] = "Graph",
  ["ric short_ui_name prop_screen_on"] = "Graph",
  ["ric shortest_ui_name prop_screen_on"] = "Grph",

  ["propertyname prop_midi_on"] = "Keyboard",
  ["ric short_ui_name prop_midi_on"] = "Keyboard",
  ["ric shortest_ui_name prop_midi_on"] = "Kbd",

  ["propertyname cv_out_1"] = "CV Out [1]",
  ["propertyname prop_cv_out_1_type"] = "CV Out Type [1]",
  ["propertyname cv_out_2"] = "CV Out [2]",
  ["propertyname prop_cv_out_2_type"] = "CV Out Type [2]",
  ["propertyname cv_out_3"] = "CV Out [3]",
  ["propertyname prop_cv_out_3_type"] = "CV Out Type [3]",
  ["propertyname cv_out_4"] = "CV Out [4]",
  ["propertyname prop_cv_out_4_type"] = "CV Out Type [4]",

  ["ui_selector prop_cv_out_type passthrough"] = "Passthrough",
  ["ui_selector prop_cv_out_type inverse"] = "Inverse",
  ["ui_selector prop_cv_out_type unipolar"] = "Unipolar",
  ["ui_selector prop_cv_out_type bipolar"] = "Bipolar",
}

for i = 1, maxArraySize do
	texts["propertyname prop_array_" .. i] = "Array [" .. i .. "]"
end
