/**************************************************************************/
/*  audio_graph_editor_plugin.cpp                                      */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "audio_graph_editor_plugin.h"

#include "animation_blend_space_1d_editor.h"
#include "animation_blend_space_2d_editor.h"
#include "animation_blend_tree_editor_plugin.h"
#include "animation_state_machine_editor.h"
#include "editor/editor_command_palette.h"
#include "editor/editor_node.h"
#include "editor/editor_settings.h"
#include "editor/gui/editor_bottom_panel.h"
#include "editor/themes/editor_scale.h"
#include "scene/animation/animation_blend_tree.h"
#include "scene/gui/button.h"
#include "scene/gui/flow_container.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/separator.h"

void AudioGraphEditor::edit(AudioStreamGraph *p_audio_graph) {
	// if (p_tree && !p_tree->is_connected("animation_list_changed", callable_mp(this, &AudioGraphEditor::_animation_list_changed))) {
	// 	p_tree->connect("animation_list_changed", callable_mp(this, &AudioGraphEditor::_animation_list_changed), CONNECT_DEFERRED);
	// }

	// if (tree == p_tree) {
	// 	return;
	// }

	// if (tree && tree->is_connected("animation_list_changed", callable_mp(this, &AudioGraphEditor::_animation_list_changed))) {
	// 	tree->disconnect("animation_list_changed", callable_mp(this, &AudioGraphEditor::_animation_list_changed));
	// }

	// tree = p_tree;

	// Vector<String> path;
	// if (tree) {
	// 	edit_path(path);
	// }
}

void AudioGraphEditor::_node_removed(Node *p_node) {
	// if (p_node == tree) {
	// 	tree = nullptr;
	// 	_clear_editors();
	// }
}

void AudioGraphEditor::_path_button_pressed(int p_path) {
	edited_path.clear();
	for (int i = 0; i <= p_path; i++) {
		edited_path.push_back(button_path[i]);
	}
}

void AudioGraphEditor::_update_path() {
}

void AudioGraphEditor::edit_path(const Vector<String> &p_path) {
	button_path.clear();

	// Ref<AnimationNode> node = tree->get_root_animation_node();

	// if (node.is_valid()) {
	// 	current_root = node->get_instance_id();

	// 	for (int i = 0; i < p_path.size(); i++) {
	// 		Ref<AnimationNode> child = node->get_child_by_name(p_path[i]);
	// 		ERR_BREAK(child.is_null());
	// 		node = child;
	// 		button_path.push_back(p_path[i]);
	// 	}

	// 	edited_path = button_path;

	// 	for (int i = 0; i < editors.size(); i++) {
	// 		if (editors[i]->can_edit(node)) {
	// 			editors[i]->edit(node);
	// 			editors[i]->show();
	// 		} else {
	// 			editors[i]->edit(Ref<AnimationNode>());
	// 			editors[i]->hide();
	// 		}
	// 	}
	// } else {
	// 	current_root = ObjectID();
	// 	edited_path = button_path;
	// 	for (int i = 0; i < editors.size(); i++) {
	// 		editors[i]->edit(Ref<AnimationNode>());
	// 		editors[i]->hide();
	// 	}
	// }

	// _update_path();
}

void AudioGraphEditor::_clear_editors() {
	button_path.clear();
	current_root = ObjectID();
	edited_path = button_path;
	for (int i = 0; i < editors.size(); i++) {
		editors[i]->edit(Ref<AudioStreamGraph>());
		editors[i]->hide();
	}
	_update_path();
}

Vector<String> AudioGraphEditor::get_edited_path() const {
	return button_path;
}

void AudioGraphEditor::enter_editor(const String &p_path) {
	Vector<String> path = edited_path;
	path.push_back(p_path);
	edit_path(path);
}

void AudioGraphEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			get_tree()->connect("node_removed", callable_mp(this, &AudioGraphEditor::_node_removed));
		} break;
		case NOTIFICATION_PROCESS: {
			ObjectID root;
			// if (tree && tree->get_root_animation_node().is_valid()) {
			// 	root = tree->get_root_animation_node()->get_instance_id();
			// }

			if (root != current_root) {
				edit_path(Vector<String>());
			}

			if (button_path.size() != edited_path.size()) {
				edit_path(edited_path);
			}
		} break;
		case NOTIFICATION_EXIT_TREE: {
			get_tree()->disconnect("node_removed", callable_mp(this, &AudioGraphEditor::_node_removed));
		} break;
	}
}

AudioGraphEditor *AudioGraphEditor::singleton = nullptr;

void AudioGraphEditor::add_plugin(AudioNodeEditorPlugin *p_editor) {
}

void AudioGraphEditor::remove_plugin(AudioNodeEditorPlugin *p_editor) {
}

String AudioGraphEditor::get_base_path() {
	String path = Animation::PARAMETERS_BASE_PATH;
	for (int i = 0; i < edited_path.size(); i++) {
		path += edited_path[i] + "/";
	}
	return path;
}

bool AudioGraphEditor::can_edit(const Ref<AudioStreamGraph> &p_node) const {
	for (int i = 0; i < editors.size(); i++) {
		if (editors[i]->can_edit(p_node)) {
			return true;
		}
	}
	return false;
}

Variant AudioGraphEditor::get_drag_data_fw(const Point2 &p_point, Control *p_from) {
	// if (p_from == members) {
	// 	TreeItem *it = members->get_item_at_position(p_point);
	// 	if (!it) {
	// 		return Variant();
	// 	}
	// 	if (!it->has_meta("id")) {
	// 		return Variant();
	// 	}

	// 	int id = it->get_meta("id");
	// 	AddOption op = add_options[id];

	// 	Dictionary d;
	// 	d["id"] = id;

	// 	Label *label = memnew(Label);
	// 	label->set_text(it->get_text(0));
	// 	label->set_auto_translate_mode(AUTO_TRANSLATE_MODE_DISABLED);
	// 	set_drag_preview(label);
	// 	return d;
	// }
	// return Variant();
}

bool AudioGraphEditor::can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const {
	if (p_from == graph) {
		Dictionary d = p_data;

		if (d.has("id")) {
			return true;
		}
		if (d.has("files")) {
			return true;
		}
	}

	return false;
}

void AudioGraphEditor::drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) {
	// if (p_from == graph) {
	// 	Dictionary d = p_data;

	// 	if (d.has("id")) {
	// 		int idx = d["id"];
	// 		saved_node_pos = p_point;
	// 		saved_node_pos_dirty = true;
	// 		_add_node(idx, add_options[idx].ops);
	// 	} else if (d.has("files")) {
	// 		EditorUndoRedoManager *undo_redo = EditorUndoRedoManager::get_singleton();
	// 		undo_redo->create_action(TTR("Add Node(s) to Visual Shader"));

	// 		if (d["files"].get_type() == Variant::PACKED_STRING_ARRAY) {
	// 			PackedStringArray arr = d["files"];
	// 			for (int i = 0; i < arr.size(); i++) {
	// 				String type = ResourceLoader::get_resource_type(arr[i]);
	// 				if (type == "GDScript") {
	// 					Ref<Script> scr = ResourceLoader::load(arr[i]);
	// 					if (scr->get_instance_base_type() == "VisualShaderNodeCustom") {
	// 						saved_node_pos = p_point + Vector2(0, i * 250 * EDSCALE);
	// 						saved_node_pos_dirty = true;

	// 						int idx = -1;

	// 						for (int j = custom_node_option_idx; j < add_options.size(); j++) {
	// 							if (add_options[j].script.is_valid()) {
	// 								if (add_options[j].script->get_path() == arr[i]) {
	// 									idx = j;
	// 									break;
	// 								}
	// 							}
	// 						}
	// 						if (idx != -1) {
	// 							_add_node(idx, {}, arr[i], i);
	// 						}
	// 					}
	// 				} else if (type == "CurveTexture") {
	// 					saved_node_pos = p_point + Vector2(0, i * 250 * EDSCALE);
	// 					saved_node_pos_dirty = true;
	// 					_add_node(curve_node_option_idx, {}, arr[i], i);
	// 				} else if (type == "CurveXYZTexture") {
	// 					saved_node_pos = p_point + Vector2(0, i * 250 * EDSCALE);
	// 					saved_node_pos_dirty = true;
	// 					_add_node(curve_xyz_node_option_idx, {}, arr[i], i);
	// 				} else if (ClassDB::get_parent_class(type) == "Texture2D") {
	// 					saved_node_pos = p_point + Vector2(0, i * 250 * EDSCALE);
	// 					saved_node_pos_dirty = true;
	// 					_add_node(texture2d_node_option_idx, {}, arr[i], i);
	// 				} else if (type == "Texture2DArray") {
	// 					saved_node_pos = p_point + Vector2(0, i * 250 * EDSCALE);
	// 					saved_node_pos_dirty = true;
	// 					_add_node(texture2d_array_node_option_idx, {}, arr[i], i);
	// 				} else if (ClassDB::get_parent_class(type) == "Texture3D") {
	// 					saved_node_pos = p_point + Vector2(0, i * 250 * EDSCALE);
	// 					saved_node_pos_dirty = true;
	// 					_add_node(texture3d_node_option_idx, {}, arr[i], i);
	// 				} else if (type == "Cubemap") {
	// 					saved_node_pos = p_point + Vector2(0, i * 250 * EDSCALE);
	// 					saved_node_pos_dirty = true;
	// 					_add_node(cubemap_node_option_idx, {}, arr[i], i);
	// 				} else if (type == "Mesh" && visual_shader->get_mode() == Shader::MODE_PARTICLES &&
	// 						(visual_shader->get_shader_type() == VisualShader::TYPE_START || visual_shader->get_shader_type() == VisualShader::TYPE_START_CUSTOM)) {
	// 					saved_node_pos = p_point + Vector2(0, i * 250 * EDSCALE);
	// 					saved_node_pos_dirty = true;
	// 					_add_node(mesh_emitter_option_idx, {}, arr[i], i);
	// 				}
	// 			}
	// 		}
	// 		undo_redo->commit_action();
	// 	}
	// }
}

// void AudioGraphEditor::_connection_request(const String &p_from, int p_from_index, const String &p_to, int p_to_index) {
// 	VisualShader::Type type = get_current_shader_type();

// 	int from = p_from.to_int();
// 	int to = p_to.to_int();

// 	if (!visual_shader->can_connect_nodes(type, from, p_from_index, to, p_to_index)) {
// 		return;
// 	}

// 	EditorUndoRedoManager *undo_redo = EditorUndoRedoManager::get_singleton();
// 	undo_redo->create_action(TTR("Nodes Connected"));

// 	List<VisualShader::Connection> conns;
// 	visual_shader->get_node_connections(type, &conns);

// 	for (const VisualShader::Connection &E : conns) {
// 		if (E.to_node == to && E.to_port == p_to_index) {
// 			undo_redo->add_do_method(visual_shader.ptr(), "disconnect_nodes", type, E.from_node, E.from_port, E.to_node, E.to_port);
// 			undo_redo->add_undo_method(visual_shader.ptr(), "connect_nodes", type, E.from_node, E.from_port, E.to_node, E.to_port);
// 			undo_redo->add_do_method(graph_plugin.ptr(), "disconnect_nodes", type, E.from_node, E.from_port, E.to_node, E.to_port);
// 			undo_redo->add_undo_method(graph_plugin.ptr(), "connect_nodes", type, E.from_node, E.from_port, E.to_node, E.to_port);
// 		}
// 	}

// 	undo_redo->add_do_method(visual_shader.ptr(), "connect_nodes", type, from, p_from_index, to, p_to_index);
// 	undo_redo->add_undo_method(visual_shader.ptr(), "disconnect_nodes", type, from, p_from_index, to, p_to_index);
// 	undo_redo->add_do_method(graph_plugin.ptr(), "connect_nodes", type, from, p_from_index, to, p_to_index);
// 	undo_redo->add_undo_method(graph_plugin.ptr(), "disconnect_nodes", type, from, p_from_index, to, p_to_index);

// 	undo_redo->add_do_method(graph_plugin.ptr(), "update_node", (int)type, from);
// 	undo_redo->add_undo_method(graph_plugin.ptr(), "update_node", (int)type, from);
// 	undo_redo->add_do_method(graph_plugin.ptr(), "update_node", (int)type, to);
// 	undo_redo->add_undo_method(graph_plugin.ptr(), "update_node", (int)type, to);
// 	undo_redo->commit_action();
// }

// void AudioGraphEditor::_disconnection_request(const String &p_from, int p_from_index, const String &p_to, int p_to_index) {
// 	graph->disconnect_node(p_from, p_from_index, p_to, p_to_index);

// 	VisualShader::Type type = get_current_shader_type();

// 	int from = p_from.to_int();
// 	int to = p_to.to_int();

// 	EditorUndoRedoManager *undo_redo = EditorUndoRedoManager::get_singleton();
// 	undo_redo->create_action(TTR("Nodes Disconnected"));
// 	undo_redo->add_do_method(visual_shader.ptr(), "disconnect_nodes", type, from, p_from_index, to, p_to_index);
// 	undo_redo->add_undo_method(visual_shader.ptr(), "connect_nodes", type, from, p_from_index, to, p_to_index);
// 	undo_redo->add_do_method(graph_plugin.ptr(), "disconnect_nodes", type, from, p_from_index, to, p_to_index);
// 	undo_redo->add_undo_method(graph_plugin.ptr(), "connect_nodes", type, from, p_from_index, to, p_to_index);
// 	undo_redo->add_do_method(graph_plugin.ptr(), "update_node", (int)type, to);
// 	undo_redo->add_undo_method(graph_plugin.ptr(), "update_node", (int)type, to);
// 	undo_redo->commit_action();
// }

void AudioGraphEditor::_show_members_dialog(bool at_mouse_pos, AudioStreamGraphNode::PortType p_input_port_type, AudioStreamGraphNode::PortType p_output_port_type) {
	if (members_input_port_type != p_input_port_type || members_output_port_type != p_output_port_type) {
		members_input_port_type = p_input_port_type;
		members_output_port_type = p_output_port_type;
		//_update_options_menu();
	}

	if (at_mouse_pos) {
		saved_node_pos_dirty = true;
		saved_node_pos = graph->get_local_mouse_position();

		Point2 gpos = get_screen_position() + get_local_mouse_position();
		members_dialog->set_position(gpos);
	} else {
		saved_node_pos_dirty = false;
		members_dialog->set_position(graph->get_screen_position() + Point2(5 * EDSCALE, 65 * EDSCALE));
	}

	if (members_dialog->is_visible()) {
		members_dialog->grab_focus();
		return;
	}

	members_dialog->popup();

	// Keep dialog within window bounds.
	Rect2 window_rect = Rect2(get_window()->get_position(), get_window()->get_size());
	Rect2 dialog_rect = Rect2(members_dialog->get_position(), members_dialog->get_size());
	Vector2 difference = (dialog_rect.get_end() - window_rect.get_end()).maxf(0);
	members_dialog->set_position(members_dialog->get_position() - difference);

	node_filter->grab_focus();
	node_filter->select_all();
}

AudioGraphEditor::AudioGraphEditor() {
	singleton = this;
	MarginContainer *main_box = memnew(MarginContainer);
	main_box->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	add_child(main_box);

	graph = memnew(GraphEdit);
	graph->set_v_size_flags(SIZE_EXPAND_FILL);
	graph->set_h_size_flags(SIZE_EXPAND_FILL);
	graph->set_custom_minimum_size(Size2(200 * EDSCALE, 0));
	graph->set_grid_pattern(GraphEdit::GridPattern::GRID_PATTERN_DOTS);
	int grid_pattern = EDITOR_GET("editors/visual_editors/grid_pattern");
	graph->set_grid_pattern((GraphEdit::GridPattern)grid_pattern);
	graph->set_show_zoom_label(true);
	main_box->add_child(graph);
	SET_DRAG_FORWARDING_GCD(graph, AudioGraphEditor);
	float graph_minimap_opacity = EDITOR_GET("editors/visual_editors/minimap_opacity");
	graph->set_minimap_opacity(graph_minimap_opacity);
	float graph_lines_curvature = EDITOR_GET("editors/visual_editors/lines_curvature");
	graph->set_connection_lines_curvature(graph_lines_curvature);

	//graph signals
	// graph->connect("connection_request", callable_mp(this, &AudioGraphEditor::_connection_request), CONNECT_DEFERRED);
	// graph->connect("disconnection_request", callable_mp(this, &AudioGraphEditor::_disconnection_request), CONNECT_DEFERRED);
	// graph->connect("node_selected", callable_mp(this, &AudioGraphEditor::_node_selected));
	// graph->connect("scroll_offset_changed", callable_mp(this, &AudioGraphEditor::_scroll_changed));
	// graph->connect("duplicate_nodes_request", callable_mp(this, &AudioGraphEditor::_duplicate_nodes));
	// graph->connect("copy_nodes_request", callable_mp(this, &AudioGraphEditor::_copy_nodes).bind(false));
	// graph->connect("cut_nodes_request", callable_mp(this, &AudioGraphEditor::_copy_nodes).bind(true));
	// graph->connect("paste_nodes_request", callable_mp(this, &AudioGraphEditor::_paste_nodes).bind(false, Point2()));
	// graph->connect("delete_nodes_request", callable_mp(this, &AudioGraphEditor::_delete_nodes_request));
	// graph->connect(SceneStringName(gui_input), callable_mp(this, &AudioGraphEditor::_graph_gui_input));
	// graph->connect("connection_to_empty", callable_mp(this, &AudioGraphEditor::_connection_to_empty));
	// graph->connect("connection_from_empty", callable_mp(this, &AudioGraphEditor::_connection_from_empty));
	// graph->connect(SceneStringName(visibility_changed), callable_mp(this, &AudioGraphEditor::_visibility_changed));

	PanelContainer *toolbar_panel = static_cast<PanelContainer *>(graph->get_menu_hbox()->get_parent());
	toolbar_panel->set_anchors_and_offsets_preset(Control::PRESET_TOP_WIDE, PRESET_MODE_MINSIZE, 10);
	toolbar_panel->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);

	HFlowContainer *toolbar = memnew(HFlowContainer);
	{
		LocalVector<Node *> toolbar_nodes;
		for (int i = 0; i < graph->get_menu_hbox()->get_child_count(); i++) {
			Node *child = graph->get_menu_hbox()->get_child(i);
			toolbar_nodes.push_back(child);
		}

		for (Node *node : toolbar_nodes) {
			graph->get_menu_hbox()->remove_child(node);
			toolbar->add_child(node);
		}

		graph->get_menu_hbox()->hide();
		toolbar_panel->add_child(toolbar);
	}

	VSeparator *vs = memnew(VSeparator);
	toolbar->add_child(vs);
	toolbar->move_child(vs, 0);

	add_node = memnew(Button);
	add_node->set_flat(true);
	add_node->set_text(TTR("Add Node..."));
	add_node->connect(SceneStringName(pressed), callable_mp(this, &AudioGraphEditor::_show_members_dialog).bind(false, AudioStreamGraphNode::PORT_TYPE_MAX, AudioStreamGraphNode::PORT_TYPE_MAX));
	toolbar->add_child(add_node);
	toolbar->move_child(add_node, 0);

	VBoxContainer *members_vb = memnew(VBoxContainer);
	members_vb->set_v_size_flags(SIZE_EXPAND_FILL);

	HBoxContainer *filter_hb = memnew(HBoxContainer);
	members_vb->add_child(filter_hb);

	node_filter = memnew(LineEdit);
	filter_hb->add_child(node_filter);
	// node_filter->connect(SceneStringName(text_changed), callable_mp(this, &AudioGraphEditor::_member_filter_changed));
	// node_filter->connect(SceneStringName(gui_input), callable_mp(this, &AudioGraphEditor::_sbox_input));
	node_filter->set_h_size_flags(SIZE_EXPAND_FILL);
	node_filter->set_placeholder(TTR("Search"));

	tools = memnew(MenuButton);
	filter_hb->add_child(tools);
	tools->set_tooltip_text(TTR("Options"));
	//tools->get_popup()->connect(SceneStringName(id_pressed), callable_mp(this, &AudioGraphEditor::_tools_menu_option));
	tools->get_popup()->add_item(TTR("Expand All"), EXPAND_ALL);
	tools->get_popup()->add_item(TTR("Collapse All"), COLLAPSE_ALL);

	members = memnew(Tree);
	members_vb->add_child(members);
	SET_DRAG_FORWARDING_GCD(members, AudioGraphEditor);
	members->set_auto_translate_mode(AUTO_TRANSLATE_MODE_DISABLED); // TODO: Implement proper translation switch.
	members->set_h_size_flags(SIZE_EXPAND_FILL);
	members->set_v_size_flags(SIZE_EXPAND_FILL);
	members->set_hide_root(true);
	members->set_allow_reselect(true);
	members->set_hide_folding(false);
	members->set_custom_minimum_size(Size2(180 * EDSCALE, 200 * EDSCALE));
	// members->connect("item_activated", callable_mp(this, &AudioGraphEditor::_member_create));
	// members->connect(SceneStringName(item_selected), callable_mp(this, &AudioGraphEditor::_member_selected));

	HBoxContainer *desc_hbox = memnew(HBoxContainer);
	members_vb->add_child(desc_hbox);

	Label *desc_label = memnew(Label);
	desc_hbox->add_child(desc_label);
	desc_label->set_text(TTR("Description:"));

	desc_hbox->add_spacer();

	highend_label = memnew(Label);
	desc_hbox->add_child(highend_label);
	highend_label->set_visible(false);
	highend_label->set_text("Vulkan");
	highend_label->set_mouse_filter(Control::MOUSE_FILTER_STOP);
	highend_label->set_tooltip_text(TTR("High-end node"));

	node_desc = memnew(RichTextLabel);
	members_vb->add_child(node_desc);
	node_desc->set_h_size_flags(SIZE_EXPAND_FILL);
	node_desc->set_v_size_flags(SIZE_FILL);
	node_desc->set_custom_minimum_size(Size2(0, 70 * EDSCALE));

	members_dialog = memnew(ConfirmationDialog);
	members_dialog->set_title(TTR("Create Audio Node"));
	members_dialog->add_child(members_vb);
	members_dialog->set_ok_button_text(TTR("Create"));
	members_dialog->connect(SceneStringName(confirmed), callable_mp(this, &AudioGraphEditor::_member_create));
	members_dialog->get_ok_button()->set_disabled(true);
	//members_dialog->connect("canceled", callable_mp(this, &AudioGraphEditor::_member_cancel));
	members_dialog->register_text_enter(node_filter);
	add_child(members_dialog);

	GraphNode *test_node = memnew(GraphNode);
	test_node->set_draggable(true);
	test_node->set_title("Output");
	graph->add_child(test_node);
}

void AudioGraphEditorPlugin::edit(Object *p_object) {
	audio_graph_editor->edit(Object::cast_to<AudioStreamGraph>(p_object));
}

bool AudioGraphEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("AudioStreamGraph");
}

void AudioGraphEditorPlugin::make_visible(bool p_visible) {
	if (p_visible) {
		button->show();
		EditorNode::get_bottom_panel()->make_item_visible(audio_graph_editor);
		audio_graph_editor->set_process(true);
	} else {
		if (audio_graph_editor->is_visible_in_tree()) {
			EditorNode::get_bottom_panel()->hide_bottom_panel();
		}
		button->hide();
		audio_graph_editor->set_process(false);
	}
}

AudioGraphEditorPlugin::AudioGraphEditorPlugin() {
	audio_graph_editor = memnew(AudioGraphEditor);
	audio_graph_editor->set_custom_minimum_size(Size2(0, 300) * EDSCALE);

	button = EditorNode::get_bottom_panel()->add_item(TTR("Audio Graph"), audio_graph_editor, ED_SHORTCUT_AND_COMMAND("bottom_panels/toggle_animation_tree_bottom_panel", TTR("Toggle AnimationTree Bottom Panel")));
	button->hide();
}

AudioGraphEditorPlugin::~AudioGraphEditorPlugin() {
}
