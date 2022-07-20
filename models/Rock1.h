// "Rock1.h" generated by "Obj2Header.exe" [Version 1.9d] Author: L.Norri Fullsail University.
// Data is converted to left-handed coordinate system and UV data is V flipped for Direct3D/Vulkan.
// The companion file "Rock1.h2b" is a binary dump of this format(-padding) for more flexibility.
// Loading *.h2b: read version, sizes, handle strings(max len 260) by reading until null-terminated.
/***********************/
/*  Generator version  */
/***********************/
#ifndef _Rock1_version_
const char Rock1_version[4] = { '0','1','9','d' };
#define _Rock1_version_
#endif
/************************************************/
/*  This section contains the model's size data */
/************************************************/
#ifndef _Rock1_vertexcount_
const unsigned Rock1_vertexcount = 102;
#define _Rock1_vertexcount_
#endif
#ifndef _Rock1_indexcount_
const unsigned Rock1_indexcount = 102;
#define _Rock1_indexcount_
#endif
#ifndef _Rock1_materialcount_
const unsigned Rock1_materialcount = 1; // can be used for batched draws
#define _Rock1_materialcount_
#endif
#ifndef _Rock1_meshcount_
const unsigned Rock1_meshcount = 1;
#define _Rock1_meshcount_
#endif
/************************************************/
/*  This section contains the raw data to load  */
/************************************************/
#ifndef __OBJ_VEC3__
typedef struct _OBJ_VEC3_
{
	float x,y,z; // 3D Coordinate.
}OBJ_VEC3;
#define __OBJ_VEC3__
#endif
#ifndef __OBJ_VERT__
typedef struct _OBJ_VERT_
{
	OBJ_VEC3 pos; // Left-handed +Z forward coordinate w not provided, assumed to be 1.
	OBJ_VEC3 uvw; // D3D/Vulkan style top left 0,0 coordinate.
	OBJ_VEC3 nrm; // Provided direct from obj file, may or may not be normalized.
}OBJ_VERT;
#define __OBJ_VERT__
#endif
#ifndef _Rock1_vertices_
// Raw Vertex Data follows: Positions, Texture Coordinates and Normals.
const OBJ_VERT Rock1_vertices[102] =
{
	{	{ 1.068558f, 0.218595f, -0.744491f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.490900f, -0.823200f, -0.285400f }	},
	{	{ 1.009496f, -0.006783f, -0.196062f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.490900f, -0.823200f, -0.285400f }	},
	{	{ 0.435884f, 0.020903f, -1.262424f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.490900f, -0.823200f, -0.285400f }	},
	{	{ 0.435884f, 0.020903f, -1.262424f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.006000f, -0.999600f, -0.029200f }	},
	{	{ 1.009496f, -0.006783f, -0.196062f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.006000f, -0.999600f, -0.029200f }	},
	{	{ -0.500396f, -0.048072f, 0.908975f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.006000f, -0.999600f, -0.029200f }	},
	{	{ -0.523697f, 0.204472f, -1.178164f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.111400f, -0.129800f, -0.985300f }	},
	{	{ -0.089962f, 0.765184f, -1.301073f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.111400f, -0.129800f, -0.985300f }	},
	{	{ 0.435884f, 0.020903f, -1.262424f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.111400f, -0.129800f, -0.985300f }	},
	{	{ -0.902296f, -0.016704f, -0.456886f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.929600f, -0.348300f, 0.121000f }	},
	{	{ -1.072526f, 0.735536f, 0.400511f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.929600f, -0.348300f, 0.121000f }	},
	{	{ -1.061383f, 0.453293f, -0.326255f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.929600f, -0.348300f, 0.121000f }	},
	{	{ 0.785341f, 1.159483f, 0.072508f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.273400f, 0.931200f, 0.241200f }	},
	{	{ -0.002372f, 1.448411f, -0.150173f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.273400f, 0.931200f, 0.241200f }	},
	{	{ -0.070240f, 1.145171f, 1.097573f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.273400f, 0.931200f, 0.241200f }	},
	{	{ 0.785341f, 1.159483f, 0.072508f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.319400f, 0.905500f, 0.279300f }	},
	{	{ -0.070240f, 1.145171f, 1.097573f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.319400f, 0.905500f, 0.279300f }	},
	{	{ 0.506445f, 0.967677f, 1.013446f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.319400f, 0.905500f, 0.279300f }	},
	{	{ -0.070240f, 1.145171f, 1.097573f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.166000f, 0.960300f, 0.224300f }	},
	{	{ -0.002372f, 1.448411f, -0.150173f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.166000f, 0.960300f, 0.224300f }	},
	{	{ -0.699597f, 1.117261f, 0.751280f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.166000f, 0.960300f, 0.224300f }	},
	{	{ -0.699597f, 1.117261f, 0.751280f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.395200f, 0.918100f, 0.031600f }	},
	{	{ -0.002372f, 1.448411f, -0.150173f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.395200f, 0.918100f, 0.031600f }	},
	{	{ -0.763145f, 1.130230f, -0.419732f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.395200f, 0.918100f, 0.031600f }	},
	{	{ -0.699597f, 1.117261f, 0.751280f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.735900f, 0.675400f, 0.047400f }	},
	{	{ -0.763145f, 1.130230f, -0.419732f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.735900f, 0.675400f, 0.047400f }	},
	{	{ -1.072526f, 0.735536f, 0.400511f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.735900f, 0.675400f, 0.047400f }	},
	{	{ -0.763145f, 1.130230f, -0.419732f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.181900f, 0.851600f, -0.491700f }	},
	{	{ -0.002372f, 1.448411f, -0.150173f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.181900f, 0.851600f, -0.491700f }	},
	{	{ -0.089962f, 0.765184f, -1.301073f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.181900f, 0.851600f, -0.491700f }	},
	{	{ -0.089962f, 0.765184f, -1.301073f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.086200f, 0.853800f, -0.513400f }	},
	{	{ -0.002372f, 1.448411f, -0.150173f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.086200f, 0.853800f, -0.513400f }	},
	{	{ 0.678066f, 0.802315f, -1.110426f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.086200f, 0.853800f, -0.513400f }	},
	{	{ -0.002372f, 1.448411f, -0.150173f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.401800f, 0.865900f, -0.297900f }	},
	{	{ 0.785341f, 1.159483f, 0.072508f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.401800f, 0.865900f, -0.297900f }	},
	{	{ 0.678066f, 0.802315f, -1.110426f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.401800f, 0.865900f, -0.297900f }	},
	{	{ 0.901900f, 0.444475f, 0.763373f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.821500f, 0.459800f, 0.337200f }	},
	{	{ 0.785341f, 1.159483f, 0.072508f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.821500f, 0.459800f, 0.337200f }	},
	{	{ 0.506445f, 0.967677f, 1.013446f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.821500f, 0.459800f, 0.337200f }	},
	{	{ -0.300295f, 0.387063f, 1.296485f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.199200f, 0.191700f, 0.961000f }	},
	{	{ 0.506445f, 0.967677f, 1.013446f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.199200f, 0.191700f, 0.961000f }	},
	{	{ -0.070240f, 1.145171f, 1.097573f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.199200f, 0.191700f, 0.961000f }	},
	{	{ -0.300295f, 0.387063f, 1.296485f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.462900f, 0.353700f, 0.812800f }	},
	{	{ -0.070240f, 1.145171f, 1.097573f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.462900f, 0.353700f, 0.812800f }	},
	{	{ -0.699597f, 1.117261f, 0.751280f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.462900f, 0.353700f, 0.812800f }	},
	{	{ -1.061383f, 0.453293f, -0.326255f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.911200f, 0.379200f, -0.161200f }	},
	{	{ -1.072526f, 0.735536f, 0.400511f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.911200f, 0.379200f, -0.161200f }	},
	{	{ -0.763145f, 1.130230f, -0.419732f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.911200f, 0.379200f, -0.161200f }	},
	{	{ -0.523697f, 0.204472f, -1.178164f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.659800f, 0.366700f, -0.655900f }	},
	{	{ -0.763145f, 1.130230f, -0.419732f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.659800f, 0.366700f, -0.655900f }	},
	{	{ -0.089962f, 0.765184f, -1.301073f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.659800f, 0.366700f, -0.655900f }	},
	{	{ 1.068558f, 0.218595f, -0.744491f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.868600f, 0.447100f, -0.213800f }	},
	{	{ 0.678066f, 0.802315f, -1.110426f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.868600f, 0.447100f, -0.213800f }	},
	{	{ 0.785341f, 1.159483f, 0.072508f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.868600f, 0.447100f, -0.213800f }	},
	{	{ 0.901900f, 0.444475f, 0.763373f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.407200f, -0.124700f, 0.904800f }	},
	{	{ 0.506445f, 0.967677f, 1.013446f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.407200f, -0.124700f, 0.904800f }	},
	{	{ -0.300295f, 0.387063f, 1.296485f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.407200f, -0.124700f, 0.904800f }	},
	{	{ 0.901900f, 0.444475f, 0.763373f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.407300f, -0.116200f, 0.905900f }	},
	{	{ -0.300295f, 0.387063f, 1.296485f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.407300f, -0.116200f, 0.905900f }	},
	{	{ 0.600219f, -0.003589f, 0.841531f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.407300f, -0.116200f, 0.905900f }	},
	{	{ 0.600219f, -0.003589f, 0.841531f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.072200f, -0.681600f, 0.728100f }	},
	{	{ -0.300295f, 0.387063f, 1.296485f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.072200f, -0.681600f, 0.728100f }	},
	{	{ -0.500396f, -0.048072f, 0.908975f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.072200f, -0.681600f, 0.728100f }	},
	{	{ -0.300295f, 0.387063f, 1.296485f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.734100f, 0.100000f, 0.671600f }	},
	{	{ -0.699597f, 1.117261f, 0.751280f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.734100f, 0.100000f, 0.671600f }	},
	{	{ -1.072526f, 0.735536f, 0.400511f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.734100f, 0.100000f, 0.671600f }	},
	{	{ -0.300295f, 0.387063f, 1.296485f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.778600f, -0.177900f, 0.601800f }	},
	{	{ -1.072526f, 0.735536f, 0.400511f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.778600f, -0.177900f, 0.601800f }	},
	{	{ -0.500396f, -0.048072f, 0.908975f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.778600f, -0.177900f, 0.601800f }	},
	{	{ -0.500396f, -0.048072f, 0.908975f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.851600f, -0.466100f, 0.239900f }	},
	{	{ -1.072526f, 0.735536f, 0.400511f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.851600f, -0.466100f, 0.239900f }	},
	{	{ -0.902296f, -0.016704f, -0.456886f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.851600f, -0.466100f, 0.239900f }	},
	{	{ -1.061383f, 0.453293f, -0.326255f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.778800f, 0.264600f, -0.568800f }	},
	{	{ -0.763145f, 1.130230f, -0.419732f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.778800f, 0.264600f, -0.568800f }	},
	{	{ -0.523697f, 0.204472f, -1.178164f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.778800f, 0.264600f, -0.568800f }	},
	{	{ -1.061383f, 0.453293f, -0.326255f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.855300f, -0.151800f, -0.495500f }	},
	{	{ -0.523697f, 0.204472f, -1.178164f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.855300f, -0.151800f, -0.495500f }	},
	{	{ -0.902296f, -0.016704f, -0.456886f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.855300f, -0.151800f, -0.495500f }	},
	{	{ -0.089962f, 0.765184f, -1.301073f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.234100f, 0.115200f, -0.965400f }	},
	{	{ 0.678066f, 0.802315f, -1.110426f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.234100f, 0.115200f, -0.965400f }	},
	{	{ 0.435884f, 0.020903f, -1.262424f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.234100f, 0.115200f, -0.965400f }	},
	{	{ 0.435884f, 0.020903f, -1.262424f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.642000f, -0.050200f, -0.765100f }	},
	{	{ 0.678066f, 0.802315f, -1.110426f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.642000f, -0.050200f, -0.765100f }	},
	{	{ 1.068558f, 0.218595f, -0.744491f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.642000f, -0.050200f, -0.765100f }	},
	{	{ 1.068558f, 0.218595f, -0.744491f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.970700f, 0.228800f, 0.073000f }	},
	{	{ 0.785341f, 1.159483f, 0.072508f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.970700f, 0.228800f, 0.073000f }	},
	{	{ 0.901900f, 0.444475f, 0.763373f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.970700f, 0.228800f, 0.073000f }	},
	{	{ 1.068558f, 0.218595f, -0.744491f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.994000f, 0.005000f, 0.109100f }	},
	{	{ 0.901900f, 0.444475f, 0.763373f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.994000f, 0.005000f, 0.109100f }	},
	{	{ 1.009496f, -0.006783f, -0.196062f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.994000f, 0.005000f, 0.109100f }	},
	{	{ 1.009496f, -0.006783f, -0.196062f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.810600f, -0.489700f, 0.321200f }	},
	{	{ 0.901900f, 0.444475f, 0.763373f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.810600f, -0.489700f, 0.321200f }	},
	{	{ 0.600219f, -0.003589f, 0.841531f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.810600f, -0.489700f, 0.321200f }	},
	{	{ 1.009496f, -0.006783f, -0.196062f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.041600f, -0.998900f, 0.019500f }	},
	{	{ 0.600219f, -0.003589f, 0.841531f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.041600f, -0.998900f, 0.019500f }	},
	{	{ -0.500396f, -0.048072f, 0.908975f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.041600f, -0.998900f, 0.019500f }	},
	{	{ -0.500396f, -0.048072f, 0.908975f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.012100f, -0.999600f, -0.026500f }	},
	{	{ -0.902296f, -0.016704f, -0.456886f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.012100f, -0.999600f, -0.026500f }	},
	{	{ 0.435884f, 0.020903f, -1.262424f },	{ 0.000000f, 0.000000f, 0.000000f },	{ 0.012100f, -0.999600f, -0.026500f }	},
	{	{ 0.435884f, 0.020903f, -1.262424f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.205900f, -0.900000f, -0.384100f }	},
	{	{ -0.902296f, -0.016704f, -0.456886f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.205900f, -0.900000f, -0.384100f }	},
	{	{ -0.523697f, 0.204472f, -1.178164f },	{ 0.000000f, 0.000000f, 0.000000f },	{ -0.205900f, -0.900000f, -0.384100f }	},
};
#define _Rock1_vertices_
#endif
#ifndef _Rock1_indices_
// Index Data follows: Sequential by mesh, Every Three Indices == One Triangle.
const unsigned int Rock1_indices[102] =
{
	 0, 1, 2,
	 3, 4, 5,
	 6, 7, 8,
	 9, 10, 11,
	 12, 13, 14,
	 15, 16, 17,
	 18, 19, 20,
	 21, 22, 23,
	 24, 25, 26,
	 27, 28, 29,
	 30, 31, 32,
	 33, 34, 35,
	 36, 37, 38,
	 39, 40, 41,
	 42, 43, 44,
	 45, 46, 47,
	 48, 49, 50,
	 51, 52, 53,
	 54, 55, 56,
	 57, 58, 59,
	 60, 61, 62,
	 63, 64, 65,
	 66, 67, 68,
	 69, 70, 71,
	 72, 73, 74,
	 75, 76, 77,
	 78, 79, 80,
	 81, 82, 83,
	 84, 85, 86,
	 87, 88, 89,
	 90, 91, 92,
	 93, 94, 95,
	 96, 97, 98,
	 99, 100, 101,
};
#define _Rock1_indices_
#endif
// Part of an OBJ_MATERIAL, the struct is 16 byte aligned so it is GPU register friendly.
#ifndef __OBJ_ATTRIBUTES__
typedef struct _OBJ_ATTRIBUTES_
{
	OBJ_VEC3    Kd; // diffuse reflectivity
	float	    d; // dissolve (transparency) 
	OBJ_VEC3    Ks; // specular reflectivity
	float       Ns; // specular exponent
	OBJ_VEC3    Ka; // ambient reflectivity
	float       sharpness; // local reflection map sharpness
	OBJ_VEC3    Tf; // transmission filter
	float       Ni; // optical density (index of refraction)
	OBJ_VEC3    Ke; // emissive reflectivity
	unsigned    illum; // illumination model
}OBJ_ATTRIBUTES;
#define __OBJ_ATTRIBUTES__
#endif
// This structure also has been made GPU register friendly so it can be transfered directly if desired.
// Note: Total struct size will vary depenedening on CPU processor architecture (string pointers).
#ifndef __OBJ_MATERIAL__
typedef struct _OBJ_MATERIAL_
{
	// The following items are typically used in a pixel/fragment shader, they are packed for GPU registers.
	OBJ_ATTRIBUTES attrib; // Surface shading characteristics & illumination model
	// All items below this line are not needed on the GPU and are generally only used during load time.
	const char* name; // the name of this material
	// If the model's materials contain any specific texture data it will be located below.
	const char* map_Kd; // diffuse texture
	const char* map_Ks; // specular texture
	const char* map_Ka; // ambient texture
	const char* map_Ke; // emissive texture
	const char* map_Ns; // specular exponent texture
	const char* map_d; // transparency texture
	const char* disp; // roughness map (displacement)
	const char* decal; // decal texture (lerps texture & material colors)
	const char* bump; // normal/bumpmap texture
	void* padding[2]; // 16 byte alignment on 32bit or 64bit
}OBJ_MATERIAL;
#define __OBJ_MATERIAL__
#endif
#ifndef _Rock1_materials_
// Material Data follows: pulled from a .mtl file of the same name if present.
const OBJ_MATERIAL Rock1_materials[1] =
{
	{
		{{ 0.122341f, 0.122341f, 0.122341f },
		1.000000f,
		{ 0.500000f, 0.500000f, 0.500000f },
		96.078430f,
		{ 1.000000f, 1.000000f, 1.000000f },
		60.000000f,
		{ 1.000000f, 1.000000f, 1.000000f },
		1.000000f,
		{ 0.000000f, 0.000000f, 0.000000f },
		2},
		"Rock",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
	},
};
#define _Rock1_materials_
#endif
/************************************************/
/*  This section contains the model's structure */
/************************************************/
#ifndef _Rock1_batches_
// Use this conveinence array to batch render all geometry by like material.
// Each entry corresponds to the same entry in the materials array above.
// The two numbers provided are the IndexCount and the IndexOffset into the indices array.
// If you need more fine grained control(ex: for transformations) use the OBJ_MESH data below.
const unsigned int Rock1_batches[1][2] =
{
	{ 102, 0 },
};
#define _Rock1_batches_
#endif
#ifndef __OBJ_MESH__
typedef struct _OBJ_MESH_
{
	const char* name;
	unsigned    indexCount;
	unsigned    indexOffset;
	unsigned    materialIndex;
}OBJ_MESH;
#define __OBJ_MESH__
#endif
#ifndef _Rock1_meshes_
// Mesh Data follows: Meshes are .obj groups sorted & split by material.
// Meshes are provided in sequential order, sorted by material first and name second.
const OBJ_MESH Rock1_meshes[1] =
{
	{
		"default",
		102,
		0,
		0,
	},
};
#define _Rock1_meshes_
#endif