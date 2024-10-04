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
class AudioStreamPlaybackGraph;

class AudioStreamGraph : public AudioStream {
	GDCLASS(AudioStreamGraph, AudioStream)
	OBJ_SAVE_TYPE(AudioStream);

public:
	struct Connection {
		int from_node = 0;
		int from_port = 0;
		int to_node = 0;
		int to_port = 0;
	};

private:
	struct Node {
		Ref<AudioStreamGraphNode> node;
		Vector2 position;
		LocalVector<int> prev_connected_nodes;
		LocalVector<int> next_connected_nodes;
	};

	struct Graph {
		RBMap<int, Node> nodes;
		List<Connection> connections;
	} graph;

protected:
	static void _bind_methods();

public:
	virtual Ref<AudioStreamPlayback> instantiate_playback() override;

	void add_node(Ref<AudioStreamGraphNode> p_node, const Vector2 &p_position, const int p_id);
	Ref<AudioStreamGraphNode> get_node(const int p_id) const;
	void remove_node(const int p_id);
	bool has_node(const int p_id) const;
	Vector<int> get_node_list() const;
	Vector2 get_node_position(const int p_id) const;
	int get_valid_node_id();
	void get_node_connections(List<Connection> *r_connections) const;
	Error connect_nodes(int p_from_node, int p_from_port, int p_to_node, int p_to_port);
	void disconnect_nodes(int p_from_node, int p_from_port, int p_to_node, int p_to_port);
	void set_node_position(int p_id, const Vector2 &p_position);

	AudioStreamGraph();
};

class AudioStreamGraphNode : public Resource {
	GDCLASS(AudioStreamGraphNode, Resource);

public:
	enum PortType {
		PORT_TYPE_STREAM,
		PORT_TYPE_MAX,
	};

	enum Category {
		CATEGORY_INPUT,
		CATEGORY_OUTPUT,
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
	virtual String get_caption() const = 0;
	virtual int get_input_port_count() const = 0;
	virtual PortType get_input_port_type(int p_port) const = 0;
	virtual String get_input_port_name(int p_port) const = 0;

	virtual int get_output_port_count() const = 0;
	virtual PortType get_output_port_type(int p_port) const = 0;
	virtual String get_output_port_name(int p_port) const = 0;

	void set_input_port_connected(int p_port, bool p_connected);
	void set_output_port_connected(int p_port, bool p_connected);

	bool is_deletable() const;
	void set_deletable(bool p_closable = true);
	AudioStreamGraphNode();
};

VARIANT_ENUM_CAST(AudioStreamGraphNode::PortType)

class AudioStreamPlaybackGraph : public AudioStreamPlayback {
	GDCLASS(AudioStreamPlaybackGraph, AudioStreamPlayback)
	friend class AudioStreamGraph;

	bool active = false;

	Ref<AudioStreamGraph> stream;

protected:
	static void _bind_methods();

public:
	virtual void start(double p_from_pos = 0.0) override;
	virtual void stop() override;
	virtual bool is_playing() const override;
	virtual int get_loop_count() const override; // times it looped
	virtual double get_playback_position() const override;
	virtual void seek(double p_time) override;
	virtual int mix(AudioFrame *p_buffer, float p_rate_scale, int p_frames) override;

	virtual void tag_used_streams() override;

	virtual void set_parameter(const StringName &p_name, const Variant &p_value) override;
	virtual Variant get_parameter(const StringName &p_name) const override;
};

#endif // AUDIO_STREAM_GRAPH_H
