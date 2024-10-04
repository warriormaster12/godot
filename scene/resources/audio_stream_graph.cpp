/**************************************************************************/
/*  audio_stream_graph.cpp                                               */
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

#include "audio_stream_graph.h"
#include "audio_stream_graph_nodes.h"

void AudioStreamGraph::_bind_methods() {
}

void AudioStreamGraph::add_node(Ref<AudioStreamGraphNode> p_node, const Vector2 &p_position, const int p_id) {
	ERR_FAIL_COND(p_node.is_null());
	ERR_FAIL_COND(p_id < 2);
	ERR_FAIL_COND(graph.nodes.has(p_id));
	Node n;
	n.node = p_node;
	n.position = p_position;

	emit_changed();
	//emit_signal(SNAME("tree_changed"));

	// p_node->connect(SNAME("tree_changed"), callable_mp(this, &AnimationNodeBlendTree::_tree_changed), CONNECT_REFERENCE_COUNTED);
	// p_node->connect(SNAME("animation_node_renamed"), callable_mp(this, &AnimationNodeBlendTree::_animation_node_renamed), CONNECT_REFERENCE_COUNTED);
	// p_node->connect(SNAME("animation_node_removed"), callable_mp(this, &AnimationNodeBlendTree::_animation_node_removed), CONNECT_REFERENCE_COUNTED);
	// p_node->connect_changed(callable_mp(this, &AnimationNodeBlendTree::_node_changed).bind(p_name), CONNECT_REFERENCE_COUNTED);

	graph.nodes[p_id] = n;
}

Error AudioStreamGraph::connect_nodes(int p_from_node, int p_from_port, int p_to_node, int p_to_port) {
	Graph *g = &graph;

	ERR_FAIL_COND_V(!g->nodes.has(p_from_node), ERR_INVALID_PARAMETER);
	ERR_FAIL_COND_V(!g->nodes.has(p_to_node), ERR_INVALID_PARAMETER);
	ERR_FAIL_INDEX_V(p_to_port, g->nodes[p_to_node].node->get_input_port_count(), ERR_INVALID_PARAMETER);


	for (const Connection &E : g->connections) {
		if (E.from_node == p_from_node && E.from_port == p_from_port && E.to_node == p_to_node && E.to_port == p_to_port) {
			ERR_FAIL_V(ERR_ALREADY_EXISTS);
		}
	}

	Connection c;
	c.from_node = p_from_node;
	c.from_port = p_from_port;
	c.to_node = p_to_node;
	c.to_port = p_to_port;
	g->connections.push_back(c);
	g->nodes[p_from_node].next_connected_nodes.push_back(p_to_node);
	g->nodes[p_to_node].prev_connected_nodes.push_back(p_from_node);
	g->nodes[p_from_node].node->set_output_port_connected(p_from_port, true);
	g->nodes[p_to_node].node->set_input_port_connected(p_to_port, true);

	//_queue_update();
	return OK;
}

void AudioStreamGraph::disconnect_nodes(int p_from_node, int p_from_port, int p_to_node, int p_to_port) {
	Graph *g = &graph;

	for (const List<Connection>::Element *E = g->connections.front(); E; E = E->next()) {
		if (E->get().from_node == p_from_node && E->get().from_port == p_from_port && E->get().to_node == p_to_node && E->get().to_port == p_to_port) {
			g->connections.erase(E);
			g->nodes[p_from_node].next_connected_nodes.erase(p_to_node);
			g->nodes[p_to_node].prev_connected_nodes.erase(p_from_node);
			g->nodes[p_from_node].node->set_output_port_connected(p_from_port, false);
			g->nodes[p_to_node].node->set_input_port_connected(p_to_port, false);
			return;
		}
	}
}


Ref<AudioStreamGraphNode> AudioStreamGraph::get_node(const int p_id) const {
	if (!graph.nodes.has(p_id)) {
		return Ref<AudioStreamGraphNode>();
	}
	ERR_FAIL_COND_V(!graph.nodes.has(p_id), Ref<AudioStreamGraphNode>());
	return graph.nodes[p_id].node;
}

void AudioStreamGraph::remove_node(const int p_id) {
	ERR_FAIL_COND(p_id < 2);
	ERR_FAIL_COND(!graph.nodes.has(p_id));

	graph.nodes.erase(p_id);

	for (List<Connection>::Element *E = graph.connections.front(); E;) {
		List<Connection>::Element *N = E->next();
		const AudioStreamGraph::Connection &connection = E->get();
		if (connection.from_node == p_id || connection.to_node == p_id) {
			if (connection.from_node == p_id) {
				graph.nodes[connection.to_node].prev_connected_nodes.erase(p_id);
				graph.nodes[connection.to_node].node->set_input_port_connected(connection.to_port, false);
			} else if (connection.to_node == p_id) {
				graph.nodes[connection.from_node].next_connected_nodes.erase(p_id);
				graph.nodes[connection.from_node].node->set_output_port_connected(connection.from_port, false);
			}
			graph.connections.erase(E);
		}
		E = N;
	}
}

bool AudioStreamGraph::has_node(const int p_id) const {
	return graph.nodes.has(p_id);
}

Vector<int> AudioStreamGraph::get_node_list() const {
	const Graph *g = &graph;

	Vector<int> ret;
	for (const KeyValue<int, Node> &E : g->nodes) {
		ret.push_back(E.key);
	}

	return ret;
}

Vector2 AudioStreamGraph::get_node_position(const int p_id) const {
	ERR_FAIL_COND_V(!graph.nodes.has(p_id), Vector2());
	return graph.nodes[p_id].position;
}

int AudioStreamGraph::get_valid_node_id() {
	const Graph *g = &graph;
	return g->nodes.size() ? MAX(2, g->nodes.back()->key() + 1) : 2;
}

void AudioStreamGraph::get_node_connections(List<Connection> *r_connections) const {
	const Graph *g = &graph;

	for (const Connection &E : g->connections) {
		r_connections->push_back(E);
	}
}

AudioStreamGraph::AudioStreamGraph() {
	Ref<AudioStreamGraphOutputNode> output;
	output.instantiate();
	graph.nodes[0].node = output;

	graph.nodes[0].position = Vector2(400, 150);
}

/////

void AudioStreamGraphNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_node", "name", "node", "position"), &AudioStreamGraph::add_node);
	ClassDB::bind_method(D_METHOD("get_node", "name"), &AudioStreamGraph::get_node);
	ClassDB::bind_method(D_METHOD("remove_node", "name"), &AudioStreamGraph::remove_node);

	ClassDB::bind_method(D_METHOD("connect_nodes", "from_node", "from_port", "to_node", "to_port"), &AudioStreamGraph::connect_nodes);
	ClassDB::bind_method(D_METHOD("disconnect_nodes", "from_node", "from_port", "to_node", "to_port"), &AudioStreamGraph::disconnect_nodes);
}

void AudioStreamGraphNode::set_input_port_connected(int p_port, bool p_connected) {
	connected_input_ports[p_port] = p_connected;
}

void AudioStreamGraphNode::set_output_port_connected(int p_port, bool p_connected) {
	if (p_connected) {
		connected_output_ports[p_port]++;
	} else {
		connected_output_ports[p_port]--;
	}
}

bool AudioStreamGraphNode::is_deletable() const {
	return closable;
}

void AudioStreamGraphNode::set_deletable(bool p_closable) {
	closable = p_closable;
}

AudioStreamGraphNode::AudioStreamGraphNode() {
}