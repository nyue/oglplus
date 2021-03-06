/**
 *  @file oglplus/shapes/twisted_torus.ipp
 *  @brief Implementation of shapes::TwistedTorus
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

namespace oglplus {
namespace shapes {

OGLPLUS_LIB_FUNC
std::vector<GLfloat> TwistedTorus::_positions(void) const
{
	typedef GLfloat T;
	std::vector<GLfloat> dest(2*2*2*_sections*(_rings + 1)*3);
	unsigned k = 0;
	//
	const double t = _thickness / _radius_in;
	const double r_twist = double(_twist) / double(_rings);
	const double r_step = (math::TwoPi()) / double(_rings);
	const double s_step = (math::TwoPi()) / double(_sections);
	const double s_slip = s_step * _s_slip_coef;
	const double r1 = _radius_in;
	const double r2 = _radius_out - _radius_in;

	for(unsigned f=0; f!=2; ++f)
	{
		const double f_sign = (f == 0)? 1.0: -1.0;
		const double fdt = t*f_sign*0.95;
		for(unsigned s=0; s!=_sections; ++s)
		{
			const double s_angle = s_step*0.5 + s*s_step;
			const double sa[2] = {
				s_angle + s_slip*f_sign,
				s_angle - s_slip*f_sign
			};
			for(unsigned r=0; r!=_rings+1; ++r)
			{
				const double r_angle = r*r_step;
				double vx = std::cos(r_angle);
				double vz = std::sin(r_angle);

				const double ta = s_step*r*r_twist;

				for(unsigned d=0; d!=2; ++d)
				{
					double vr = std::cos(sa[d]+ta);
					double vy = std::sin(sa[d]+ta);

					dest[k++] = T(vx*(r1 + r2*(1.0 + vr) + fdt*vr));
					dest[k++] = T(vy*(r2 + fdt));
					dest[k++] = T(vz*(r1 + r2*(1.0 + vr) + fdt*vr));
				}
			}
		}
	}

	for(unsigned d=0; d!=2; ++d)
	{
		const double d_sign = (d == 0)? 1.0: -1.0;
		for(unsigned s=0; s!=_sections; ++s)
		{
			const double s_angle = s_step*0.5 + s*s_step;
			const double sa = s_angle + s_slip*d_sign;
			for(unsigned r=0; r!=_rings+1; ++r)
			{
				const double r_angle = r*r_step;
				const double ta = s_step*r*r_twist;
				const double vr = std::cos(sa+ta);
				const double vy = std::sin(sa+ta);

				double vx = std::cos(r_angle);
				double vz = std::sin(r_angle);

				for(unsigned f=0; f!=2; ++f)
				{
					const double f_sign = (f == 0)? 1.0: -1.0;
					const double fdt = -t*d_sign*f_sign*0.95;

					dest[k++] = T(vx*(r1 + r2*(1.0 + vr) + fdt*vr));
					dest[k++] = T(vy*(r2 + fdt));
					dest[k++] = T(vz*(r1 + r2*(1.0 + vr) + fdt*vr));
				}
			}
		}
	}
	assert(k == dest.size());
	return dest;
}

OGLPLUS_LIB_FUNC
std::vector<GLfloat> TwistedTorus::_normals(void) const
{
	typedef GLfloat T;
	std::vector<GLfloat> dest(2*2*2*_sections*(_rings + 1)*3);
	unsigned k = 0;
	//
	const double r_twist= double(_twist) / double(_rings);
	const double r_step = (math::TwoPi()) / double(_rings);
	const double s_step = (math::TwoPi()) / double(_sections);
	const double s_slip = s_step * _s_slip_coef;

	for(unsigned f=0; f!=2; ++f)
	{
		const double f_sign = (f == 0)? 1.0: -1.0;
		for(unsigned s=0; s!=_sections; ++s)
		{
			const double s_angle = s_step*0.5 + s*s_step;
			const double sa[2] = {
				s_angle + s_slip*f_sign,
				s_angle - s_slip*f_sign
			};
			for(unsigned r=0; r!=_rings+1; ++r)
			{
				const double r_angle = r*r_step;
				double vx = std::cos(r_angle);
				double vz = std::sin(r_angle);

				const double ta = s_step*r*r_twist;

				for(unsigned d=0; d!=2; ++d)
				{
					double vr = std::cos(sa[d]+ta);
					double vy = std::sin(sa[d]+ta);

					dest[k++] = T(f_sign*vx*vr);
					dest[k++] = T(f_sign*vy);
					dest[k++] = T(f_sign*vz*vr);
				}
			}
		}
	}

	for(unsigned d=0; d!=2; ++d)
	{
		const double d_sign = (d == 0)? 1.0: -1.0;
		for(unsigned s=0; s!=_sections; ++s)
		{
			const double s_angle = s_step*0.5 + s*s_step;
			const double sa = s_angle + s_slip*d_sign;
			for(unsigned r=0; r!=_rings+1; ++r)
			{
				const double ta = s_step*r*r_twist;
				const double vr = std::sin(sa+ta);
				const double vy = std::cos(sa+ta);
				const double r_angle = r*r_step;
				double vx = std::cos(r_angle);
				double vz = std::sin(r_angle);

				for(unsigned f=0; f!=2; ++f)
				{
					dest[k++] = T(d_sign*-vx*vr);
					dest[k++] = T(d_sign*vy);
					dest[k++] = T(d_sign*-vz*vr);
				}
			}
		}
	}

	assert(k == dest.size());
	return dest;
}

OGLPLUS_LIB_FUNC
std::vector<GLfloat> TwistedTorus::_tangents(void) const
{
	typedef GLfloat T;
	std::vector<T> poss;
	Positions(poss);

	std::vector<GLfloat> dest(2*2*2*_sections*(_rings + 1)*3);

	assert(dest.size() == poss.size());
	unsigned k = 0;
	//
	for(unsigned f=0; f!=2; ++f)
	{
		unsigned foff = f*_sections*(_rings+1)*2;
		for(unsigned s=0; s!=_sections; ++s)
		{
			unsigned s0 = s*(_rings+1)*2;
			for(unsigned r=0; r!=_rings+1; ++r)
			{
				unsigned s1 = s0;
				unsigned r0 = r;
				unsigned r1 = r+1;
				if(r == _rings)
				{
					s1 = ((s+_twist)%_sections)*
						(_rings+1)*2;
					r1 = 1;
				}

				for(unsigned d=0; d!=2; ++d)
				{
					std::size_t k0 = foff+s0+r0*2+d;
					std::size_t k1 = foff+s1+r1*2+d;

					T tx = poss[k1*3+0]-poss[k0*3+0];
					T ty = poss[k1*3+1]-poss[k0*3+1];
					T tz = poss[k1*3+2]-poss[k0*3+2];
					T tl = std::sqrt(tx*tx+ty*ty+tz*tz);

					assert(tl > T(0));

					dest[k++] = tx/tl;
					dest[k++] = ty/tl;
					dest[k++] = tz/tl;
				}
			}
		}
	}

	for(unsigned d=0; d!=2; ++d)
	{
		std::size_t doff = d*_sections*(_rings+1)*2;
		for(unsigned s=0; s!=_sections; ++s)
		{
			unsigned s0 = s*(_rings+1)*2;
			for(unsigned r=0; r!=_rings+1; ++r)
			{
				unsigned s1 = s0;
				unsigned r0 = r;
				unsigned r1 = r+1;
				if(r == _rings)
				{
					s1 = ((s+_twist)%_sections)*
						(_rings+1)*2;
					r1 = 1;
				}

				for(unsigned f=0; f!=2; ++f)
				{
					std::size_t k0 = doff+s0+r0*2+f;
					std::size_t k1 = doff+s1+r1*2+f;

					T tx = poss[k1*3+0]-poss[k0*3+0];
					T ty = poss[k1*3+1]-poss[k0*3+1];
					T tz = poss[k1*3+2]-poss[k0*3+2];
					T tl = std::sqrt(tx*tx+ty*ty+tz*tz);

					assert(tl > T(0));

					dest[k++] = tx/tl;
					dest[k++] = ty/tl;
					dest[k++] = tz/tl;
				}
			}
		}
	}

	assert(k == dest.size());
	return dest;
}

OGLPLUS_LIB_FUNC
std::vector<GLfloat> TwistedTorus::_bitangents(void) const
{
	typedef GLfloat T;
	std::vector<T> nmls, tgts;
	Normals(nmls);
	Tangents(tgts);
	assert(nmls.size() == tgts.size());
	assert(nmls.size() % 3 == 0);

	std::vector<GLfloat> dest(nmls.size());

	unsigned k = 0;

	while(k != dest.size())
	{
		T nx = nmls[k+0];
		T ny = nmls[k+1];
		T nz = nmls[k+2];

		T tx = tgts[k+0];
		T ty = tgts[k+1];
		T tz = tgts[k+2];

		dest[k++] = T(ny*tz-nz*ty);
		dest[k++] = T(nz*tx-nx*tz);
		dest[k++] = T(nx*ty-ny*tx);
	}

	assert(k == dest.size());
	return dest;
}

OGLPLUS_LIB_FUNC
std::vector<GLfloat> TwistedTorus::_tex_coords(void) const
{
	typedef GLfloat T;
	std::vector<GLfloat> dest(2*2*2*_sections*(_rings + 1)*2);
	unsigned k = 0;
	//
	double t = _thickness / _radius_in;
	double r_step = 0.5 / double(_rings);
	double s_step = 1.0 / double(_sections);
	double s_slip = s_step * t;

	s_slip = s_step * _s_slip_coef;

	for(unsigned f=0; f!=2; ++f)
	{
		const double f_sign = (f == 0)? 1.0: -1.0;
		for(unsigned s=0; s!=_sections; ++s)
		{
			const double s_angle = s_step*0.5 + s*s_step;
			const double sa[2] = {
				s_angle + s_slip*f_sign,
				s_angle - s_slip*f_sign
			};
			for(unsigned r=0; r!=_rings+1; ++r)
			{
				const double r_angle = 2*r*r_step;
				double u = r_angle;
				for(unsigned d=0; d!=2; ++d)
				{
					double v = sa[d];
					dest[k++] = T(u);
					dest[k++] = T(v);
				}
			}
		}
	}

	for(unsigned d=0; d!=2; ++d)
	{
		const double d_sign = (d == 0)? 1.0: -1.0;
		for(unsigned s=0; s!=_sections; ++s)
		{
			const double s_angle = s_step*0.5 + s*s_step;
			const double v = s_angle + s_slip*d_sign;
			for(unsigned r=0; r!=_rings+1; ++r)
			{
				const double r_angle = 2*r*r_step;
				double u = r_angle;
				for(unsigned f=0; f!=2; ++f)
				{
					dest[k++] = T(u);
					dest[k++] = T(v);
				}
			}
		}
	}

	assert(k == dest.size());
	return dest;
}

OGLPLUS_LIB_FUNC
DrawingInstructions
TwistedTorus::Instructions(TwistedTorus::Default) const
{
	auto instructions = this->MakeInstructions();
	unsigned strip = 2*(_rings + 1);
	unsigned offs = 0;

	GLuint phase = 0;

	for(unsigned f=0; f!=4; ++f)
	{
		for(unsigned s=0; s!=_sections; ++s)
		{
			DrawOperation operation;
			operation.method = DrawOperation::Method::DrawArrays;
			operation.mode = PrimitiveType::TriangleStrip;
			operation.first = GLuint(offs);
			operation.count = GLuint(strip);
			operation.restart_index = DrawOperation::NoRestartIndex();
			operation.phase = phase;
			this->AddInstruction(instructions, operation);
			offs += strip;
		}
		++phase;
	}
	return instructions;
}

} // shapes
} // oglplus

