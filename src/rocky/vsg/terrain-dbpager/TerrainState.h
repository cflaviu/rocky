/**
 * rocky c++
 * Copyright 2023 Pelican Mapping
 * MIT License
 */
#pragma once

#include <rocky/vsg/Common.h>
#include <rocky/vsg/terrain/TerrainTileNode.h>

#include <vsg/io/Options.h>
#include <vsg/utils/GraphicsPipelineConfigurator.h>
#include <vsg/utils/ShaderSet.h>
#include <vsg/utils/SharedObjects.h>
#include <vsg/nodes/StateGroup.h>

namespace ROCKY_NAMESPACE
{
    class Runtime;
    class TerrainTileRenderModel;

    /**
     * TerrainState creates all the Vulkan state necessary to
     * render the terrain.
     *
     * TODO: Eventually, this will need to integrate "upwards" to the 
     * MapNode and finally to the application level itself so we
     * do shader composition with some kind of uber-shader-with-defines
     * architecture.
     */
    class ROCKY_VSG_INTERNAL TerrainState
    {
    public:
        //! Initialize the factory
        TerrainState(Runtime&);

        //! Creates a state group for rendering terrain
        vsg::ref_ptr<vsg::StateGroup> createTerrainStateGroup();

        //! Creates a state group for rendering a specific terrain tile
        void updateTerrainTileDescriptors(
            const TerrainTileRenderModel& renderModel,
            vsg::ref_ptr<vsg::StateGroup> stategroup,
            Runtime& runtime) const;

        TerrainTileRenderModel inheritTerrainTileDescriptors(
            const TerrainTileRenderModel& input,
            const glm::dmat4& scaleBias,
            vsg::ref_ptr<vsg::StateGroup> stategroup,
            Runtime& runtime) const;

        //! Status of the factory.
        Status status;

    public:

        //! Config object for creating the terrain's graphics pipeline
        vsg::ref_ptr<vsg::GraphicsPipelineConfig> pipelineConfig;

        //! VSG parent shader set that we use to develop the terrain tile
        //! state group for each tile.
        vsg::ref_ptr<vsg::ShaderSet> shaderSet;

        //! Default state descriptors for a terrain tile.
        //! This holds the "default" (i.e. empty) textures and uniforms
        //! that will populate a descriptor set when no other textures are available.
        //! Terrain tiles copy and use this until new data becomes available.
        TerrainTileDescriptors defaultTileDescriptors;

    protected:

        //! Creates all the default texture information,
        //! i.e. placeholder textures and uniforms for all tiles
        //! when they don't have actual data.
        void createDefaultDescriptors();

        //! Creates the base shader set used when rendering terrain
        vsg::ref_ptr<vsg::ShaderSet> createShaderSet() const;

        //! Creates a configurator for graphics pipeline state groups.
        //! The configurator does not contain any ACTUAL decriptors (like
        //! textures and uniforms) but rather just prepares the ShaderSet
        //! to work with the specific decriptors you PLAN to provide.
        vsg::ref_ptr<vsg::GraphicsPipelineConfig> createPipelineConfig() const;

        //! Defines a single texutre and its (possible shared) sampler
        struct TextureDef
        {
            // name in the shader
            std::string name;

            // binding point (layout binding=X) in the shader
            uint32_t uniform_binding;

            // sampler to use
            vsg::ref_ptr<vsg::Sampler> sampler;

            // default placeholder texture data
            vsg::ref_ptr<vsg::Data> defaultData;
        };

        //! stock samplers to use for terrain textures
        struct
        {
            TextureDef color;
            TextureDef colorParent;
            TextureDef elevation;
            TextureDef normal;
        }
        texturedefs;

        Runtime& _runtime;
    };
}
