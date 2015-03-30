#include <vector>
#include <map>
#include <unordered_map>

#include <glm/glm.hpp>

#include "utils/vboindexer.h"

#include <string.h> // for memcmp


// Returns true iif v1 can be considered equal to v2
bool is_near(float v1, float v2){
	return fabs( v1-v2 ) < 0.01f;
}

// Searches through all already-exported vertices
// for a similar one.
// Similar = same position + same UVs + same normal
bool getSimilarVertexIndex( 
	glm::vec3 & in_vertex, 
	glm::vec2 & in_uv, 
	glm::vec3 & in_normal, 
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals,
	unsigned int & result
){
	// Lame linear search
	for ( unsigned int i=0; i<out_vertices.size(); i++ ){
		if (
			is_near( in_vertex.x , out_vertices[i].x ) &&
			is_near( in_vertex.y , out_vertices[i].y ) &&
			is_near( in_vertex.z , out_vertices[i].z ) &&
			is_near( in_uv.x     , out_uvs     [i].x ) &&
			is_near( in_uv.y     , out_uvs     [i].y ) &&
			is_near( in_normal.x , out_normals [i].x ) &&
			is_near( in_normal.y , out_normals [i].y ) &&
			is_near( in_normal.z , out_normals [i].z )
		){
			result = i;
			return true;
		}
	}
	// No other vertex could be used instead.
	// Looks like we'll have to add it to the VBO.
	return false;
}

void indexVBO_slow(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,

	std::vector<unsigned int> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
){
	// For each input vertex
	for ( unsigned int i=0; i<in_vertices.size(); i++ ){

		// Try to find a similar vertex in out_XXXX
		unsigned int index;
		bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i],     out_vertices, out_uvs, out_normals, index);

		if ( found ){ // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back( index );
		}else{ // If not, it needs to be added in the output data.
			out_vertices.push_back( in_vertices[i]);
			out_uvs     .push_back( in_uvs[i]);
			out_normals .push_back( in_normals[i]);
			out_indices .push_back( (unsigned int)out_vertices.size() - 1 );
		}
	}
}

struct PackedVertex{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const PackedVertex that) const{
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
	};
};

bool getSimilarVertexIndex_fast( 
	PackedVertex & packed, 
	std::map<PackedVertex,unsigned int> & VertexToOutIndex,
	unsigned int & result
){
	std::map<PackedVertex,unsigned int>::iterator it = VertexToOutIndex.find(packed);
	if ( it == VertexToOutIndex.end() ){
		return false;
	}else{
		result = it->second;
		return true;
	}
}

void indexVBO(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,

	std::vector<unsigned int> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
){
	std::map<PackedVertex,unsigned int> VertexToOutIndex;

	// For each input vertex
	for ( unsigned int i=0; i<in_vertices.size(); i++ ){

		PackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};
		

		// Try to find a similar vertex in out_XXXX
		unsigned int index;
		bool found = getSimilarVertexIndex_fast( packed, VertexToOutIndex, index);

		if ( found ){ // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back( index );
		}else{ // If not, it needs to be added in the output data.
			out_vertices.push_back( in_vertices[i]);
			out_uvs     .push_back( in_uvs[i]);
			out_normals .push_back( in_normals[i]);
			unsigned int newindex = (unsigned int)out_vertices.size() - 1;
			out_indices .push_back( newindex );
			VertexToOutIndex[ packed ] = newindex;
		}
	}
}


//!loic: changing the way to lookup vertices to make it faster
// comparison operators for glm::vec/*{{{*/
bool is_near(const glm::vec3 &vecA, const glm::vec3 &vecB) 
{ 
    return  is_near(vecA[0], vecB[0])
        and is_near(vecA[1], vecB[1])
        and is_near(vecA[2], vecB[2]);
} 

bool is_near(const glm::vec2 &vecA, const glm::vec2 &vecB) 
{ 
    return  is_near(vecA[0], vecB[0])
        and is_near(vecA[1], vecB[1]);
} 
/*}}}*/

struct PackedVertexPTNUV/*{{{*/
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 uv;

    PackedVertexPTNUV(glm::vec3 position=glm::vec3(), glm::vec3 normal=glm::vec3(), glm::vec3 tangent=glm::vec3(), glm::vec2 uv=glm::vec2()):position(position), normal(normal), tangent(tangent), uv(uv){}
    bool operator==(const PackedVertexPTNUV& other) const
    {
        return is_near(position, other.position) and is_near(normal, other.normal) and is_near(tangent, other.tangent) and is_near(uv, other.uv);
    }
};/*}}}*/

// Template specialization of std::hash {{{
namespace std{
template<>
class hash<PackedVertexPTNUV>
{
public:
    std::size_t operator()(const PackedVertexPTNUV& v) const 
    {
        std::size_t hashes[]={
            std::hash<float>()(v.position.x),
            std::hash<float>()(v.position.y),
            std::hash<float>()(v.position.z),
            std::hash<float>()(v.normal.x),
            std::hash<float>()(v.normal.y),
            std::hash<float>()(v.normal.z),
            std::hash<float>()(v.tangent.x),
            std::hash<float>()(v.tangent.y),
            std::hash<float>()(v.tangent.z),
            std::hash<float>()(v.uv.x),
            std::hash<float>()(v.uv.y)};
        std::size_t seed=0;
        for(std::size_t h : hashes)
        {
            // from boost::hash_combine
            seed ^= h + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
}/*}}}*/

void indexVBO_TBN(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,
	std::vector<glm::vec3> & in_tangents,
	std::vector<glm::vec3> & in_bitangents,

	std::vector<unsigned int> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals,
	std::vector<glm::vec3> & out_tangents,
	std::vector<glm::vec3> & out_bitangents
){
    std::unordered_map<PackedVertexPTNUV, unsigned int> vert_indices;
	// For each input vertex
	for ( unsigned int i=0; i<in_vertices.size(); i++ ){

		// Try to find a similar vertex in out_XXXX
		unsigned int index;
        PackedVertexPTNUV v(in_vertices[i], in_normals[i], in_tangents[i], in_uvs[i]);
        bool found = vert_indices.find(v)!=vert_indices.end();
		if ( found ){ // A similar vertex is already in the VBO, use it instead !
            index=vert_indices[v];
			out_indices.push_back( index );
		}else{ // If not, it needs to be added in the output data.
			out_vertices.push_back( in_vertices[i]);
			out_uvs     .push_back( in_uvs[i]);
			out_normals .push_back( in_normals[i]);
			out_tangents .push_back( in_tangents[i]);
			out_bitangents .push_back( in_bitangents[i]);
            index=(unsigned int)out_vertices.size() - 1;
			out_indices.push_back(index);
            vert_indices[v]=index;
		}
	}
}
// vim: set foldmethod=marker :
