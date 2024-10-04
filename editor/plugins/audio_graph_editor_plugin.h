/**************************************************************************/
/*  audio_graph_editor_plugin.h                                        */
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

#ifndef AUDIO_GRAPH_EDITOR_PLUGIN
#define AUDIO_GRAPH_EDITOR_PLUGIN

#include "editor/plugins/editor_plugin.h"
#include "scene/gui/graph_edit.h"
#include "scene/resources/audio_stream_graph.h"

class Button;
class EditorFileDialog;
class ScrollContainer;
class Tree;
class AcceptDialog;
class LineEdit;
class RichTextLabel;
class MenuButton;
class ConfirmationDialog;
class GraphEdit;
class ConfirmationDialog;

class AudioNodeEditorPlugin : public VBoxContainer {
	GDCLASS(AudioNodeEditorPlugin, VBoxContainer);

public:
	virtual bool can_edit(const Ref<AudioStreamGraph> &p_node) = 0;
	virtual void edit(const Ref<AudioStreamGraph> &p_node) = 0;
};

class AudioGraphEditor : public Control {
	GDCLASS(AudioGraphEditor, Control);

	Vector<String> button_path;
	Vector<String> edited_path;
	Vector<AudioNodeEditorPlugin *> editors;

	void _update_path();
	void _clear_editors();
	ObjectID current_root;

	void _path_button_pressed(int p_path);

	GraphEdit *graph = nullptr;
	Button *add_node = nullptr;
	Tree *members = nullptr;
	Tree *parameters = nullptr;
	AcceptDialog *alert = nullptr;
	LineEdit *node_filter = nullptr;
	RichTextLabel *node_desc = nullptr;
	Label *highend_label = nullptr;
	MenuButton *tools = nullptr;

	Point2 saved_node_pos;
	bool saved_node_pos_dirty = false;
	ConfirmationDialog *members_dialog = nullptr;
	AudioStreamGraphNode::PortType members_input_port_type = AudioStreamGraphNode::PORT_TYPE_MAX;
	AudioStreamGraphNode::PortType members_output_port_type = AudioStreamGraphNode::PORT_TYPE_MAX;

	Variant get_drag_data_fw(const Point2 &p_point, Control *p_from);
	bool can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	void drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);
	void _show_members_dialog(bool at_mouse_pos, AudioStreamGraphNode::PortType p_input_port_type = AudioStreamGraphNode::PORT_TYPE_MAX, AudioStreamGraphNode::PortType p_output_port_type = AudioStreamGraphNode::PORT_TYPE_MAX);

	void _member_create();
	enum ToolsMenuOptions {
		EXPAND_ALL,
		COLLAPSE_ALL
	};

protected:
	void _notification(int p_what);
	void _node_removed(Node *p_node);

	static AudioGraphEditor *singleton;

public:
	void add_plugin(AudioNodeEditorPlugin *p_editor);
	void remove_plugin(AudioNodeEditorPlugin *p_editor);

	String get_base_path();

	bool can_edit(const Ref<AudioStreamGraph> &p_node) const;

	void edit_path(const Vector<String> &p_path);
	Vector<String> get_edited_path() const;

	void enter_editor(const String &p_path = "");
	static AudioGraphEditor *get_singleton() { return singleton; }
	void edit(AudioStreamGraph *p_audio_graph);
	AudioGraphEditor();
};

class AudioGraphEditorPlugin : public EditorPlugin {
	GDCLASS(AudioGraphEditorPlugin, EditorPlugin);

	AudioGraphEditor *audio_graph_editor = nullptr;
	Button *button = nullptr;

public:
	virtual String get_name() const override { return "AudioGraph"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	AudioGraphEditorPlugin();
	~AudioGraphEditorPlugin();
};

#endif // AUDIO_GRAPH_EDITOR_PLUGIN