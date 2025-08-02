// from x3dgp.snowfall
// GLSL Noise Algorithms
// https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
float Random(float n)
{
    return frac(sin(n) * 43758.5453123);
}
float Noise(float p)
{
    float fl = floor(p);
    float fc = frac(p);
    return lerp(Random(fl), Random(fl + 1.0), fc);
}
float Random(float2 n)
{
    return frac(sin(dot(n, float2(12.9898, 4.1414))) * 43758.5453);
}
float Noise(float2 p)
{
    float2 ip = floor(p);
    float2 u = frac(p);
    u = u * u * (3.0 - 2.0 * u);
	
    float res = lerp(
		lerp(Random(ip), Random(ip + float2(1.0, 0.0)), u.x),
		lerp(Random(ip + float2(0.0, 1.0)), Random(ip + float2(1.0, 1.0)), u.x), u.y);
    return res * res;
}

float Mod289(float x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}
float3 Mod289(float3 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}
float4 Mod289(float4 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}
float4 Perm(float4 x)
{
    return Mod289(((x * 34.0) + 1.0) * x);
}
float Noise(float3 p)
{
    float3 a = floor(p);
    float3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    float4 b = a.xxyy + float4(0.0, 1.0, 0.0, 1.0);
    float4 k1 = Perm(b.xyxy);
    float4 k2 = Perm(k1.xyxy + b.zzww);

    float4 c = k2 + a.zzzz;
    float4 k3 = Perm(c);
    float4 k4 = Perm(c + 1.0);

    float4 o1 = frac(k3 * (1.0 / 41.0));
    float4 o2 = frac(k4 * (1.0 / 41.0));

    float4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    float2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}

// Classic Perlin Noise
float4 Permute(float4 x)
{
    return fmod(((x * 34.0) + 1.0) * x, 289.0);
}
float4 TaylorInvSqrt(float4 r)
{
    return 1.79284291400159 - 0.85373472095314 * r;
}
float3 Fade(float3 t)
{
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}
float CNoise(float3 P)
{
    float3 Pi0 = floor(P); // Integer part for indexing
    float3 Pi1 = Pi0 + 1.0; // Integer part + 1
    Pi0 = fmod(Pi0, 289.0);
    Pi1 = fmod(Pi1, 289.0);
    float3 Pf0 = frac(P); // Fractional part for interpolation
    float3 Pf1 = Pf0 - 1.0; // Fractional part - 1.0
    float4 ix = float4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
    float4 iy = float4(Pi0.yy, Pi1.yy);
    float4 iz0 = Pi0.zzzz;
    float4 iz1 = Pi1.zzzz;

    float4 ixy = Permute(Permute(ix) + iy);
    float4 ixy0 = Permute(ixy + iz0);
    float4 ixy1 = Permute(ixy + iz1);

    float4 gx0 = ixy0 / 7.0;
    float4 gy0 = frac(floor(gx0) / 7.0) - 0.5;
    gx0 = frac(gx0);
    float4 gz0 = 0.5 - abs(gx0) - abs(gy0);
    float4 sz0 = step(gz0, 0.0);
    gx0 -= sz0 * (step(0.0, gx0) - 0.5);
    gy0 -= sz0 * (step(0.0, gy0) - 0.5);

    float4 gx1 = ixy1 / 7.0;
    float4 gy1 = frac(floor(gx1) / 7.0) - 0.5;
    gx1 = frac(gx1);
    float4 gz1 = 0.5 - abs(gx1) - abs(gy1);
    float4 sz1 = step(gz1, 0.0);
    gx1 -= sz1 * (step(0.0, gx1) - 0.5);
    gy1 -= sz1 * (step(0.0, gy1) - 0.5);

    float3 g000 = float3(gx0.x, gy0.x, gz0.x);
    float3 g100 = float3(gx0.y, gy0.y, gz0.y);
    float3 g010 = float3(gx0.z, gy0.z, gz0.z);
    float3 g110 = float3(gx0.w, gy0.w, gz0.w);
    float3 g001 = float3(gx1.x, gy1.x, gz1.x);
    float3 g101 = float3(gx1.y, gy1.y, gz1.y);
    float3 g011 = float3(gx1.z, gy1.z, gz1.z);
    float3 g111 = float3(gx1.w, gy1.w, gz1.w);

    float4 norm0 = TaylorInvSqrt(float4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
    g000 *= norm0.x;
    g010 *= norm0.y;
    g100 *= norm0.z;
    g110 *= norm0.w;
    float4 norm1 = TaylorInvSqrt(float4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
    g001 *= norm1.x;
    g011 *= norm1.y;
    g101 *= norm1.z;
    g111 *= norm1.w;

    float n000 = dot(g000, Pf0);
    float n100 = dot(g100, float3(Pf1.x, Pf0.yz));
    float n010 = dot(g010, float3(Pf0.x, Pf1.y, Pf0.z));
    float n110 = dot(g110, float3(Pf1.xy, Pf0.z));
    float n001 = dot(g001, float3(Pf0.xy, Pf1.z));
    float n101 = dot(g101, float3(Pf1.x, Pf0.y, Pf1.z));
    float n011 = dot(g011, float3(Pf0.x, Pf1.yz));
    float n111 = dot(g111, Pf1);

    float3 fadeXyz = Fade(Pf0);
    float4 nZ = lerp(float4(n000, n100, n010, n110), float4(n001, n101, n011, n111), fadeXyz.z);
    float2 nYZ = lerp(nZ.xy, nZ.zw, fadeXyz.y);
    float nXYZ = lerp(nYZ.x, nYZ.y, fadeXyz.x);
    return 2.2 * nXYZ;
}
// Simplex Noise
float SNoise(float3 v)
{
    const float2 C = float2(1.0 / 6.0, 1.0 / 3.0);
    const float4 D = float4(0.0, 0.5, 1.0, 2.0);

    // First corner
    float3 i = floor(v + dot(v, C.yyy));
    float3 x0 = v - i + dot(i, C.xxx);

    // Other corners
    float3 g = step(x0.yzx, x0.xyz);
    float3 l = 1.0 - g;
    float3 i1 = min(g.xyz, l.zxy);
    float3 i2 = max(g.xyz, l.zxy);

    //   x0 = x0 - 0.0 + 0.0 * C.xxx;
    //   x1 = x0 - i1  + 1.0 * C.xxx;
    //   x2 = x0 - i2  + 2.0 * C.xxx;
    //   x3 = x0 - 1.0 + 3.0 * C.xxx;
    float3 x1 = x0 - i1 + C.xxx;
    float3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
    float3 x3 = x0 - D.yyy; // -1.0+3.0*C.x = -0.5 = -D.y

  // Permutations
    i = Mod289(i);
    float4 p = Permute(Permute(Permute(
        i.z + float4(0.0, i1.z, i2.z, 1.0))
        + i.y + float4(0.0, i1.y, i2.y, 1.0))
        + i.x + float4(0.0, i1.x, i2.x, 1.0));

    // Gradients: 7x7 points over a square, mapped onto an octahedron.
    // The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
    float n_ = 0.142857142857; // 1.0/7.0
    float3 ns = n_ * D.wyz - D.xzx;

    float4 j = p - 49.0 * floor(p * ns.z * ns.z); //  mod(p,7*7)

    float4 x_ = floor(j * ns.z);
    float4 y_ = floor(j - 7.0 * x_); // mod(j,N)

    float4 x = x_ * ns.x + ns.yyyy;
    float4 y = y_ * ns.x + ns.yyyy;
    float4 h = 1.0 - abs(x) - abs(y);

    float4 b0 = float4(x.xy, y.xy);
    float4 b1 = float4(x.zw, y.zw);

    //float4 s0 = float4(lessThan(b0,0.0))*2.0 - 1.0;
    //float4 s1 = float4(lessThan(b1,0.0))*2.0 - 1.0;
    float4 s0 = floor(b0) * 2.0 + 1.0;
    float4 s1 = floor(b1) * 2.0 + 1.0;
    float4 sh = -step(h, 0.0);

    float4 a0 = b0.xzyw + s0.xzyw * sh.xxyy;
    float4 a1 = b1.xzyw + s1.xzyw * sh.zzww;

    float3 p0 = float3(a0.xy, h.x);
    float3 p1 = float3(a0.zw, h.y);
    float3 p2 = float3(a1.xy, h.z);
    float3 p3 = float3(a1.zw, h.w);

    //Normalise gradients
    float4 norm = TaylorInvSqrt(float4(dot(p0, p0), dot(p1, p1), dot(p2, p2), dot(p3, p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;

    // Mix final noise value
    float4 m = max(0.5 - float4(dot(x0, x0), dot(x1, x1), dot(x2, x2), dot(x3, x3)), 0.0);
    m = m * m;
    return 105.0 * dot(m * m, float4(dot(p0, x0), dot(p1, x1),
        dot(p2, x2), dot(p3, x3)));
}
#if 0
// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint hash(uint x)
{
    x += (x << 10u);
    x ^= (x >> 6u);
    x += (x << 3u);
    x ^= (x >> 11u);
    x += (x << 15u);
    return x;
}
// Compound versions of the hashing algorithm I whipped together.
uint hash(uint2 v)
{
    return hash(v.x ^ hash(v.y));
}
uint hash(uint3 v)
{
    return hash(v.x ^ hash(v.y) ^ hash(v.z));
}
uint hash(uint4 v)
{
    return hash(v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w));
}
// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float float_construct(uint m)
{
    const uint ieee_mantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieee_one = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieee_mantissa; // Keep only mantissa bits (fractional part)
    m |= ieee_one; // Add fractional part to 1.0

    float f = asfloat(m); // Range [1:2]
    return f - 1.0; // Range [0:1]
}
// Pseudo-random value in half-open range [0:1].
float random(float x)
{
    return float_construct(hash(asuint(x)));
}
float random(float2 v)
{
    return float_construct(hash(asuint(v)));
}
float random(float3 v)
{
    return float_construct(hash(asuint(v)));
}
float random(float4 v)
{
    return float_construct(hash(asuint(v)));
}
#endif