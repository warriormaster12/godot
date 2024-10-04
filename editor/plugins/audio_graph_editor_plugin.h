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

class AudioGraphEditorPlugin;

class AudioGraphEditor : public Control {
	GDCLASS(AudioGraphEditor, Control);
	friend class AudioGraphEditorPlugin;

	Vector<String> button_path;
	Vector<String> edited_path;

	void _update_path();
	void _clear_editors();
	ObjectID current_root;

	void _path_button_pressed(int p_path);

	AudioGraphEditorPlugin *graph_plugin = nullptr;

	GraphEdit *graph = nullptr;
	Button *add_node = nullptr;
	Tree *members = nullptr;
	Tree *parameters = nullptr;
	AcceptDialog *alert = nullptr;
	LineEdit *node_filter = nullptr;
	RichTextLabel *node_desc = nullptr;
	MenuButton *tools = nullptr;
	PopupMenu *popup_menu = nullptr;
	PopupMenu *connection_popup_menu = nullptr;

	Point2 saved_node_pos;
	Vector2 menu_point;
	bool saved_node_pos_dirty = false;
	bool connection_node_insert_requested = false;
	bool updating = false;
	Ref<GraphEdit::Connection> clicked_connection;
	int to_node = -1;
	int to_slot = -1;
	int from_node = -1;
	int from_slot = -1;
	ConfirmationDialog *members_dialog = nullptr;
	AudioStreamGraphNode::PortType members_input_port_type = AudioStreamGraphNode::PORT_TYPE_MAX;
	AudioStreamGraphNode::PortType members_output_port_type = AudioStreamGraphNode::PORT_TYPE_MAX;

	AudioStreamGraph *audio_graph = nullptr;

	Variant get_drag_data_fw(const Point2 &p_point, Control *p_from);
	bool can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	void drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);
	void _show_members_dialog(bool at_mouse_pos, AudioStreamGraphNode::PortType p_input_port_type = AudioStreamGraphNode::PORT_TYPE_MAX, AudioStreamGraphNode::PortType p_output_port_type = AudioStreamGraphNode::PORT_TYPE_MAX);

	void _member_create();
	void _member_selected();
	void _member_cancel();
	void _update_options_menu();
	void _add_node(int p_idx);
	void _sbox_input(const Ref<InputEvent> &p_event);
	void _graph_gui_input(const Ref<InputEvent> &p_event);
	void _node_menu_id_pressed(int p_idx);
	void _connection_menu_id_pressed(int p_idx);
	void _connection_request(const String &p_from, int p_from_index, const String &p_to, int p_to_index);
	void _disconnection_request(const String &p_from, int p_from_index, const String &p_to, int p_to_index);
	void _update_graph();


	struct CopyItem {
		int id;
		Ref<AudioStreamGraphNode> node;
		Vector2 position;
		bool disabled = false;
	};

	enum ToolsMenuOptions {
		EXPAND_ALL,
		COLLAPSE_ALL
	};

	enum NodeMenuOptions {
		ADD,
		SEPARATOR, // ignore
		CUT,
		COPY,
		PASTE,
		DELETE,
		DUPLICATE,
		CLEAR_COPY_BUFFER,
	};

	enum ConnectionMenuOptions {
		INSERT_NEW_NODE,
		INSERT_NEW_REROUTE,
		DISCONNECT,
	};

	struct AddOption {
		String name;
		String type;
		String description;
		Vector<Variant> ops;
		Ref<Script> script;
		int mode = 0;
		int return_type = 0;
		int func = 0;
		bool is_custom = false;
		bool is_native = false;
		int temp_idx = 0;

		AddOption(const String &p_name = String(), const String &p_type = String(), const String &p_description = String(), const Vector<Variant> &p_ops = Vector<Variant>(), int p_return_type = -1, int p_mode = -1) {
			name = p_name;
			type = p_type;
			description = p_description;
			ops = p_ops;
			return_type = p_return_type;
			mode = p_mode;
		}
	};
	// struct _OptionComparator {
	// 	_FORCE_INLINE_ bool operator()(const AddOption &a, const AddOption &b) const {
	// 		return a.category.count("/") > b.category.count("/") || (a.category + "/" + a.name).naturalnocasecmp_to(b.category + "/" + b.name) < 0;
	// 	}
	// };

	Vector<AddOption> add_options;

	static Vector2 selection_center;
	static List<CopyItem> copy_items_buffer;
	static List<AudioGraphEditor::Connection> copy_connections_buffer;

protected:
	void _notification(int p_what);

	static AudioGraphEditor *singleton;

public:
	String get_base_path();

	bool can_edit(const Ref<AudioStreamGraph> &p_node) const;

	void edit_path(const Vector<String> &p_path);
	Vector<String> get_edited_path() const;

	void enter_editor(const String &p_path = "");
	static AudioGraphEditor *get_singleton() { return singleton; }
	void edit(AudioStreamGraph *p_audio_graph);
	AudioGraphEditor();
};

class AGGraphNode : public GraphNode {
	GDCLASS(AGGraphNode, GraphNode);

protected:
	void _draw_port(int p_slot_index, Point2i p_pos, bool p_left, const Color &p_color, const Color &p_rim_color);
	virtual void draw_port(int p_slot_index, Point2i p_pos, bool p_left, const Color &p_color) override;
};

class AGRerouteNode : public AGGraphNode {
	GDCLASS(AGRerouteNode, GraphNode);

	const float FADE_ANIMATION_LENGTH_SEC = 0.3;

	float icon_opacity = 0.0;

protected:
	void _notification(int p_what);

	virtual void draw_port(int p_slot_index, Point2i p_pos, bool p_left, const Color &p_color) override;

public:
	AGRerouteNode();
	void set_icon_opacity(float p_opacity);

	void _on_mouse_entered();
	void _on_mouse_exited();
};

class AudioGraphEditorPlugin : public EditorPlugin {
	GDCLASS(AudioGraphEditorPlugin, EditorPlugin);

	AudioGraphEditor *audio_graph_editor = nullptr;
	Button *button = nullptr;

	struct Link {
		AudioStreamGraphNode *audio_node = nullptr;
		GraphElement *graph_element = nullptr;
	};

	HashMap<int, Link> links;

protected:
	static void _bind_methods();

public:
	void clear_links();
	void register_link(int p_id, AudioStreamGraphNode *p_audio_node, GraphElement *p_graph_element);
	bool has_main_screen() const override { return false; }
	void add_node(int p_id, bool p_just_update);
	void update_node(int p_node_id);
	void remove_node(int p_id, bool p_just_update);
	void connect_nodes(int p_from_node, int p_from_port, int p_to_node, int p_to_port);
	void disconnect_nodes(int p_from_node, int p_from_port, int p_to_node, int p_to_port);

	virtual String get_name() const override { return "AudioGraph"; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	AudioGraphEditorPlugin();
	~AudioGraphEditorPlugin();
};

#endif // AUDIO_GRAPH_EDITOR_PLUGIN