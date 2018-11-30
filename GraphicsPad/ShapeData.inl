
template<class TVert>
ShapeDataTemplate<TVert>::ShapeDataTemplate() : 
	verts(0), numVerts(0), indices(0), numIndices(0), 
	textureFileName(0), normalMapFileName(0), underlyingBuffer(0) {}

template<class TVert>
uint ShapeDataTemplate<TVert>::vertexBufferSize() const { return numVerts * sizeof(TVert); }

template<class TVert>
uint ShapeDataTemplate<TVert>::indexBufferSize() const { return numIndices * sizeof(ushort); }

template<class TVert>
uint ShapeDataTemplate<TVert>::totalBufferSize() const
{
	return vertexBufferSize() + indexBufferSize();
}

template<class TVert>
bool ShapeDataTemplate<TVert>::isValid()
{
	return underlyingBuffer != 0 || (verts != 0 && indices != 0);
}

template<class TVert>
void ShapeDataTemplate<TVert>::cleanUp()
{
	if(underlyingBuffer != 0)
	{
		delete [] underlyingBuffer;
	}
	else
	{
		delete[] verts;
		delete[] indices;
	}
	if(textureFileName != NULL)
	{
		delete[] textureFileName;
		textureFileName = 0;
	}
	numVerts = numIndices = 0;
	verts = 0;
	indices = 0;
}
