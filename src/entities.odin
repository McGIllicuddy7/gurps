#+private
package main
import "base:runtime"
import "core:strings"
import "vendor:raylib"
Vector3 :: raylib.Vector3
Vector2 :: raylib.Vector2
Quaternion :: raylib.Quaternion
Transform :: raylib.Transform
MAX_ENTITY_COUNT :: 16384

EntityKind::enum{}
EntityVTable ::struct{
	type_info: typeid,
	typename:string,
	on_physics_tick:proc(et:^Entity, delta_time:f32),
	on_tick:proc(et:^Entity, delta_time:f32),
	on_damage:proc(et:^Entity, damage_amount:f32, hit_location:string),
	destructor:proc(et:^Entity)
}

EntityBoundingBox::struct{
	width,length,height:f32, 
}

Entity ::struct {
	vtable:^EntityVTable,
	position:Vector3,
	rotation:Quaternion,
	bounding_box:EntityBoundingBox,
	mesh:EntityMesh,
}

EntityBone::struct{
	width,length,height:f32,	
	offset_from_parent:Vector3,
	children:[]EntityBone	
}

EntityMesh::struct{}