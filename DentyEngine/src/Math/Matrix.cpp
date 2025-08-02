#include "stdafx.h"

#include "Matrix.h"

const DentyEngine::Matrix DentyEngine::Matrix::IDENTITY = Matrix(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
);