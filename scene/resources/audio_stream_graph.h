/**************************************************************************/
/*  audio_stream_graph.h                                             */
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

#ifndef AUDIO_STREAM_GRAPH_H
#define AUDIO_STREAM_GRAPH_H

#include "core/templates/local_vector.h"
#include "scene/scene_string_names.h"
#include "servers/audio/audio_stream.h"
#include "servers/audio_server.h"

class AudioStreamGraphNode;

class AudioStreamGraph : public AudioStream {
	GDCLASS(AudioStreamGraph, AudioStream)

	struct Node {
		Ref<AudioStreamGraphNode> node;
		Vector2 position;
		Vector<StringName> connections;
	};

	RBMap<StringName, Node, StringName::AlphCompare> nodes;

protected:
	static void _bind_methods();

public:
	void add_node(const StringName &p_name, Ref<AudioStreamGraphNode> p_node, const Vector2 &p_position = Vector2());
	Ref<AudioStreamGraphNode> get_node(const StringName &p_name) const;
	void remove_node(const StringName &p_name);
	void rename_node(const StringName &p_name, const StringName &p_new_name);
	bool has_node(const StringName &p_name) const;
	void get_node_list(List<StringName> *r_list);

	AudioStreamGraph();
};

class AudioStreamGraphNode : public Resource {
	GDCLASS(AudioStreamGraphNode, Resource);

public:
	enum PortType {
		PORT_TYPE_SCALAR,
		PORT_TYPE_MAX,
	};

	enum Category {
		CATEGORY_OUTPUT,
		CATEGORY_INPUT,
	};

private:
	int port_preview = -1;
	int linked_parent_graph_frame = -1;

	HashMap<int, bool> connected_input_ports;
	HashMap<int, int> connected_output_ports;
	HashMap<int, bool> expanded_output_ports;

	Ref<AudioStream> stream;

protected:
	HashMap<int, Variant> default_input_values;
	bool simple_decl = true;
	bool disabled = false;
	bool closable = false;

	static void _bind_methods();

public:
	AudioStreamGraphNode();
};

VARIANT_ENUM_CAST(AudioStreamGraphNode::PortType)

#endif // AUDIO_STREAM_GRAPH_H
