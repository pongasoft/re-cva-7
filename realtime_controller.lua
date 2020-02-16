format_version = "1.0"

rtc_bindings = {
  -- this will initialize the C++ object
  { source = "/environment/system_sample_rate", dest = "/global_rtc/init_instance" },
}

global_rtc = {
  init_instance = function(source_property_path, new_value)
    local sample_rate = jbox.load_property("/environment/system_sample_rate")
    local new_no = jbox.make_native_object_rw("Instance", { sample_rate })
    jbox.store_property("/custom_properties/instance", new_no);
  end,
}

rt_input_setup = {
  notify = {

    "/cv_inputs/cv_in_1/connected",
    "/cv_inputs/cv_in_1/value",
    "/cv_outputs/cv_out_1/connected",
    "/cv_outputs/cv_out_2/connected",
    "/cv_outputs/cv_out_3/connected",
    "/cv_outputs/cv_out_4/connected",
    "/cv_outputs/cv_out_value_1/connected",
    "/cv_outputs/cv_out_min_value_1/connected",
    "/cv_outputs/cv_out_max_value_1/connected",
    "/custom_properties/prop_cv_in_1_min_max_reset",
    "/custom_properties/prop_input_paused",
    "/custom_properties/prop_input_page_offset",
    "/custom_properties/prop_input_history_offset",
    "/custom_properties/prop_zoom_factor_x",
    "/custom_properties/prop_zoom_factor_y",
    "/custom_properties/prop_cv_in_1_min_max_auto_reset",
    "/custom_properties/prop_screen_on",
    "/custom_properties/prop_midi_on",
    "/custom_properties/prop_cv_out_1_type",
    "/custom_properties/prop_cv_out_2_type",
    "/custom_properties/prop_cv_out_3_type",
    "/custom_properties/prop_cv_out_4_type",
  }
}


sample_rate_setup = {
	native = {
		22050,
		44100,
		48000,
		88200,
		96000,
		192000
	},

}
