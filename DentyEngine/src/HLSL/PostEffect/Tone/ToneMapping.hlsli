struct ToneMappingFunctions
{
    // Reference https://techblog.sega.jp/entry/ngs_hdr_techblog_202211#512-%E3%83%88%E3%83%BC%E3%83%B3%E3%83%9E%E3%83%83%E3%83%94%E3%83%B3%E3%82%B0%E3%81%A8%E3%82%AC%E3%83%B3%E3%83%9E%E8%A3%9C%E6%AD%A3%E5%87%A6%E7%90%86%E3%81%AB%E3%81%A4%E3%81%84%E3%81%A6
    static float3 ACESFilmic(float3 rgb, float a = 3.0f, float b = 0.03f, float c = 2.43f, float d = 0.59f, float e = 0.14f)
    {
        return (rgb * (a * rgb + b)) / (rgb * (c * rgb + d) + e);
    }
};