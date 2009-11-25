/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_Common.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_RTTLayer.h"
//#include "MyGUI_RTTLayerNode.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_Gui.h"
#include <Ogre.h>

namespace MyGUI
{
	// Get the mesh information for the given mesh.
	// Code found in Wiki: www.ogre3d.org/wiki/index.php/RetrieveVertexData
	void GetMeshInformation(const Ogre::MeshPtr mesh,
									size_t &vertex_count,
									Ogre::Vector3* &vertices,
									size_t &index_count,
									unsigned long* &indices,
									Ogre::Vector2* &coords,
									const Ogre::Vector3 &position,
									const Ogre::Quaternion &orient,
									const Ogre::Vector3 &scale)
	{
		bool added_shared = false;
		size_t current_offset = 0;
		size_t shared_offset = 0;
		size_t next_offset = 0;
		size_t index_offset = 0;

		vertex_count = index_count = 0;

		// Calculate how many vertices and indices we're going to need
		for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
		{
			Ogre::SubMesh* submesh = mesh->getSubMesh( i );

			// We only need to add the shared vertices once
			if(submesh->useSharedVertices)
			{
				if( !added_shared )
				{
					vertex_count += mesh->sharedVertexData->vertexCount;
					added_shared = true;
				}
			}
			else
			{
				vertex_count += submesh->vertexData->vertexCount;
			}

			// Add the indices
			index_count += submesh->indexData->indexCount;
		}


		// Allocate space for the vertices and indices
		vertices = new Ogre::Vector3[vertex_count];
		indices = new unsigned long[index_count];
		coords = new Ogre::Vector2[vertex_count];

		added_shared = false;

		// Run through the submeshes again, adding the data into the arrays
		for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
		{
			Ogre::SubMesh* submesh = mesh->getSubMesh(i);

			Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

			if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
			{
				if(submesh->useSharedVertices)
				{
					added_shared = true;
					shared_offset = current_offset;
				}

				const Ogre::VertexElement* coordElem =
					vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES);

				const Ogre::VertexElement* posElem =
					vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

				Ogre::HardwareVertexBufferSharedPtr vbuf =
					vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

				unsigned char* vertex =
					static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

				// There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
				//  as second argument. So make it float, to avoid trouble when Ogre::Real will
				//  be comiled/typedefed as double:
				//      Ogre::Real* pReal;
				float* pReal;

				for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
				{
					posElem->baseVertexPointerToElement(vertex, &pReal);

					Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

					vertices[current_offset + j] = (orient * (pt * scale)) + position;

					posElem->baseVertexPointerToElement(vertex + coordElem->getOffset() - posElem->getOffset(), &pReal);
					Ogre::Vector2 coord(pReal[0], pReal[1]);
					coords[current_offset + j] = coord;
				}

				vbuf->unlock();
				next_offset += vertex_data->vertexCount;
			}


			Ogre::IndexData* index_data = submesh->indexData;
			size_t numTris = index_data->indexCount / 3;
			Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

			bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

			unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


			size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

			// Ogre 1.6 patch (commenting the static_cast...) - index offsets start from 0 for each submesh
			if ( use32bitindexes )
			{
				for ( size_t k = 0; k < numTris*3; ++k)
				{
					indices[index_offset++] = pLong[k] /*+ static_cast<unsigned long>(offset)*/;
				}
			}
			else
			{
				for ( size_t k = 0; k < numTris*3; ++k)
				{
					indices[index_offset++] = static_cast<unsigned long>(pShort[k]) /*+
						static_cast<unsigned long>(offset)*/;
				}
			}

			ibuf->unlock();
			current_offset = next_offset;
		}
	} 

	Ogre::RaySceneQuery* gRaySceneQuery = 0;
	const int gWidth = 512;
	const int gHeight = 512;

	Ogre::SceneManager* getSceneManager()
	{
		return Ogre::Root::getSingleton().getSceneManager("BaseSceneManager");
	}

	Ogre::Camera* getCamera()
	{
		return getSceneManager()->getCamera("BaseCamera");
	}

	RTTLayer::RTTLayer() :
		mTexture(nullptr),
		mTextureCoords(nullptr),
		mVertices(nullptr),
		mVertexCount(0),
		mIndices(nullptr),
		mIndexCount(0)
	{
		std::string name = MyGUI::utility::toString((int)this, "_RTTLayer");
		mTexture = MyGUI::RenderManager::getInstance().createTexture(name);
		mTexture->createManual(gWidth, gHeight, MyGUI::TextureUsage::RenderTarget, MyGUI::PixelFormat::R8G8B8A8);

		gRaySceneQuery = getSceneManager()->createRayQuery(Ogre::Ray());
	}

	RTTLayer::~RTTLayer()
	{
		clear();

		getSceneManager()->destroyQuery(gRaySceneQuery);
		gRaySceneQuery = nullptr;

		MyGUI::RenderManager::getInstance().destroyTexture(mTexture);
		mTexture = nullptr;
	}

	void RTTLayer::clear()
	{
		if (mTextureCoords != nullptr)
		{
			delete mTextureCoords;
			mTextureCoords = nullptr;
		}

		if (mVertices != nullptr)
		{
			delete mVertices;
			mVertices = nullptr;
			mVertexCount = 0;
		}

		if (mIndices != nullptr)
		{
			delete mIndices;
			mIndices = nullptr;
			mIndexCount = 0;
		}
	}

	void RTTLayer::renderToTarget(IRenderTarget* _target, bool _update)
	{
		MyGUI::IRenderTarget* target = mTexture->getRenderTarget();
		if (target != nullptr)
		{
			target->begin();

			for (VectorILayerNode::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
			{
				if (target != nullptr)
					(*iter)->renderToTarget(target, _update);
			}

			target->end();
		}
	}

	bool RTTLayer::isIntersectMesh(IntPoint& _point, const Ogre::Ray& _ray)
	{
		Ogre::Real closest_distance = -1.0f;
		Ogre::Vector3 closest_result;

		// test for hitting individual triangles on the mesh
        bool new_closest_found = false;
		int index_found = 0;
        for (int i = 0; i < static_cast<int>(mIndexCount); i += 3)
        {
            // check for a hit against this triangle
            std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(_ray, mVertices[mIndices[i]],
                mVertices[mIndices[i+1]], mVertices[mIndices[i+2]], true, false);

            // if it was a hit check if its the closest
            if (hit.first)
            {
                if ((closest_distance < 0.0f) ||
                    (hit.second < closest_distance))
                {
                    // this is the closest so far, save it off
                    closest_distance = hit.second;
					index_found = i;
                    new_closest_found = true;
                }
            }
        }

		if (new_closest_found)
        {
            closest_result = _ray.getPoint(closest_distance);               
        }

		// return the result
		if (closest_distance >= 0.0f)
		{
			// raycast success
			//result = closest_result;
			getCoordByTriangle(_point, closest_result, mVertices[mIndices[index_found]], mVertices[mIndices[index_found+1]], mVertices[mIndices[index_found+2]]);
			return true;
		}
		else
		{
			// raycast failed
			return false;
		} 

		return false;
	}

	void RTTLayer::getCoordByTriangle(IntPoint& _point, Ogre::Vector3 _position, const Ogre::Vector3& _corner0, const Ogre::Vector3& _corner1, const Ogre::Vector3& _corner2)
	{
		Ogre::Vector2 result; // ���������

		Ogre::Vector3 dirX = _corner1 - _corner0;
		Ogre::Vector3 dirY = _corner2 - _corner0;

		_position -= _corner0; // ���������� �� ������ ��������� (�� ����� 0)

		float div = (dirY * dirX).x;
		if (div != 0.0)
		{
			result = Ogre::Vector2((_position * dirY).x, (dirX * _position).x);
			result /= div;
		}
		else if ((dirY * dirX).y != 0.0)
		{
			div = (dirY * dirX).y;
			result = Ogre::Vector2((_position * dirY).y, (dirX * _position).y);
			result /= div;
		}
		else if ((dirY * dirX).z != 0.0)
		{
			div = (dirY * dirX).z;
			result = Ogre::Vector2((_position * dirY).z, (dirX * _position).z);
			result /= div;
		}
		else
		{
			// �����
		}

		//return result;

		MyGUI::MYGUI_OUT(result.x, " - ", result.y);
	}

	ILayerItem* RTTLayer::getLayerItemByPoint(int _left, int _top)
	{
		MyGUI::IntSize size = MyGUI::Gui::getInstance().getViewSize();
		Ogre::Ray ray = getCamera()->getCameraToViewportRay(
			_left / float(size.width),
			_top / float(size.height));

		gRaySceneQuery->setRay(ray);
		gRaySceneQuery->setSortByDistance(true);
		Ogre::RaySceneQueryResult &result = gRaySceneQuery->execute();
		for (Ogre::RaySceneQueryResult::iterator iter = result.begin(); iter!=result.end(); ++iter)
		{
			if (iter->movable != 0)
			{
				if (iter->movable->getName() == mEntityName)
				{
					//Ogre::Vector3 pos = ray.getPoint(iter->distance);
					IntPoint point;
					if (isIntersectMesh(point, ray))
						return Base::getLayerItemByPoint(point.left, point.top);
				}
			}
		}

		return nullptr;
	}

	/*
	0 -- 1
	|      |
	|      |
	2 -- 3
	*/
	/*Ogre::Vector2 getRelPosition(
		const Ogre::Vector3& _corner0,
		const Ogre::Vector3& _corner1,
		const Ogre::Vector3& _corner2,
		const Ogre::Vector3& _corner3,
		Ogre::Vector3 _position)
	{
		Ogre::Vector2 result; // ���������

		Ogre::Vector3 dirX = _corner1 - _corner0;
		Ogre::Vector3 dirY = _corner2 - _corner0;

		_position -= _corner0; // ���������� �� ������ ��������� (�� ����� 0)

		float div = (dirY * dirX).x;
		if (div != 0.0)
		{
			result = Ogre::Vector2((_position * dirY).x, (dirX * _position).x);
			result /= div;
		}
		else if ((dirY * dirX).y != 0.0)
		{
			div = (dirY * dirX).y;
			result = Ogre::Vector2((_position * dirY).y, (dirX * _position).y);
			result /= div;
		}
		else if ((dirY * dirX).z != 0.0)
		{
			div = (dirY * dirX).z;
			result = Ogre::Vector2((_position * dirY).z, (dirX * _position).z);
			result /= div;
		}
		else
		{
			// �����
		}

		return result;
	}*/

	IntPoint RTTLayer::getPosition(int _left, int _top)
	{
		/*MyGUI::IntSize size = MyGUI::Gui::getInstance().getViewSize();
		Ogre::Ray ray = getCamera()->getCameraToViewportRay(
			_left / float(size.width),
			_top / float(size.height));

		gRaySceneQuery->setRay(ray);
		gRaySceneQuery->setSortByDistance(true);
		Ogre::RaySceneQueryResult &result = gRaySceneQuery->execute();
		for (Ogre::RaySceneQueryResult::iterator iter = result.begin(); iter!=result.end(); ++iter)
		{
			if (iter->movable != 0)
			{
				if (iter->movable->getName() == "FloorPlane")
				{
					Ogre::Vector3 pos = ray.getPoint(iter->distance);

					Ogre::Vector2 result = getRelPosition(
						Ogre::Vector3(-256, 0, -256),
						Ogre::Vector3(256, 0, -256),
						Ogre::Vector3(-256, 0, 256),
						Ogre::Vector3(256, 0, 256),
						pos);

					return IntPoint((int)(result.x * 512.0), (int)(result.y * 512.0));
				}
			}
		}*/

		return IntPoint(-11111, -11111);
	}

	void RTTLayer::setEntity(const std::string& _name, const std::string& _material)
	{
		clear();

		mEntityName = _name;
		Ogre::Entity* entity = getSceneManager()->getEntity(_name);
		if (entity != nullptr)
		{
			mVertexCount = 0;
			mIndexCount = 0;
			GetMeshInformation(entity->getMesh(), mVertexCount, mVertices, mIndexCount, mIndices, mTextureCoords, Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, Ogre::Vector3::UNIT_SCALE);
			
			Ogre::MaterialPtr material = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(_material);
			if (!material.isNull())
			{
				material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(mTexture->getName());
			}
		}
	}

} // namespace MyGUI
