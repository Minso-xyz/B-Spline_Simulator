#include "SurfacePreset.h"
#include "BSplineSurface.h"
#include <vector>


void ApplyWavePreset(BSplineSurface& surface)
{
	surface.DegreeU = 3;
	surface.DegreeV = 3;

	surface.ControlNet =
	{
		{
			Point3D(0, 20, 0),
			Point3D(10, 40, 0),
			Point3D(20, 15, 0),
			Point3D(30, 35, 0)
		},

		{
			Point3D(0, 35, 10),
			Point3D(10, 55, 10),
			Point3D(20, 30, 10),
			Point3D(30, 45, 10)
		},

		{
			Point3D(0, 15, 20),
			Point3D(10, 35, 20),
			Point3D(20, 10, 20),
			Point3D(30, 25, 20)
		},

		{
			Point3D(0, 30, 30),
			Point3D(10, 50, 30),
			Point3D(20, 25, 30),
			Point3D(30, 40, 30)
		}
	};
}

void ApplyGaussianPreset(BSplineSurface& surface)
{
	surface.DegreeU = 3;
	surface.DegreeV = 3;

	surface.ControlNet =
	{
		{
			Point3D(0, 12, 0),
			Point3D(10, 16, 0),
			Point3D(20, 16, 0),
			Point3D(30, 12, 0)
		},

		{
			Point3D(0, 16, 10),
			Point3D(10, 48, 10),
			Point3D(20, 48, 10),
			Point3D(30, 16, 10)
		},

		{
			Point3D(0, 16, 20),
			Point3D(10, 48, 20),
			Point3D(20, 48, 20),
			Point3D(30, 16, 20)
		},

		{
			Point3D(0, 12, 30),
			Point3D(10, 16, 30),
			Point3D(20, 16, 30),
			Point3D(30, 12, 30)
		}
	};
}

void ApplyFlatPreset(BSplineSurface& surface)
{
	surface.DegreeU = 3;
	surface.DegreeV = 3;

	surface.ControlNet =
	{
		{
			Point3D(0, 20, 0),
			Point3D(10, 20, 0),
			Point3D(20, 20, 0),
			Point3D(30, 20, 0)
		},

		{
			Point3D(0, 20, 10),
			Point3D(10, 20, 10),
			Point3D(20, 20, 10),
			Point3D(30, 20, 10)
		},

		{
			Point3D(0, 20, 20),
			Point3D(10, 20, 20),
			Point3D(20, 20, 20),
			Point3D(30, 20, 20)
		},

		{
			Point3D(0, 20, 30),
			Point3D(10, 20, 30),
			Point3D(20, 20, 30),
			Point3D(30, 20, 30)
		}
	};
}

void ApplyDomePreset(BSplineSurface& surface)
{
	surface.DegreeU = 3;
	surface.DegreeV = 3;

	surface.KnotsU =
	{
		0,0,0,0,
		1,1,1,1
	};

	surface.KnotsV =
	{
		0,0,0,0,
		1,1,1,1
	};

	surface.ControlNet =
	{
		{
			Point3D(0, 15, 0),
			Point3D(10, 18, 0),
			Point3D(20, 18, 0),
			Point3D(30, 15, 0)
		},

		{
			Point3D(0, 25, 10),
			Point3D(10, 40, 10),
			Point3D(20, 40, 10),
			Point3D(30, 25, 10)
		},

		{
			Point3D(0, 25, 20),
			Point3D(10, 40, 20),
			Point3D(20, 40, 20),
			Point3D(30, 25, 20)
		},

		{
			Point3D(0, 15, 30),
			Point3D(10, 18, 30),
			Point3D(20, 18, 30),
			Point3D(30, 15, 30)
		}
	};
}
