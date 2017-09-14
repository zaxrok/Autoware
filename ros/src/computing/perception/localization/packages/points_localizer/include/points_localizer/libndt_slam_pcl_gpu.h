/*
 *  Copyright (c) 2017, Tier IV, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither the name of Autoware nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef LIBNDT_SLAM_GPU_H
#define LIBNDT_SLAM_GPU_H

#include "libndt_slam_base.h"

#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <fast_pcl/ndt_gpu/NormalDistributionsTransform.h>

template <class PointSource, class PointTarget>
class LibNdtSlamPCLGPU
    : public LibNdtSlamBase<PointSource, PointTarget>
{
    public:
        LibNdtSlamPCLGPU();
        ~LibNdtSlamPCLGPU() = default;

        void setTransformationEpsilon(double trans_eps) override;
        void setStepSize(double step_size) override;
        void setResolution(float res) override;
        void setMaximumIterations(int max_iter) override;

        double getTransformationEpsilon() override;
        double getStepSize() const override;
        float getResolution() const override;
        int getMaximumIterations() override;

    protected:
        void align(const Pose& predict_pose) override;
        double getFitnessScore() override;
        void setInputTarget(const boost::shared_ptr< pcl::PointCloud<PointTarget> const>& map_ptr) override;
        void setInputSource(const boost::shared_ptr< pcl::PointCloud<PointSource> const>& scan_ptr) override;
        Pose getFinalPose() override;

    private:
        gpu::GNormalDistributionsTransform ndt_;
};

template <class PointSource, class PointTarget>
LibNdtSlamPCLGPU<PointSource, PointTarget>::LibNdtSlamPCLGPU()
{
}

template <class PointSource, class PointTarget>
void LibNdtSlamPCLGPU<PointSource, PointTarget>::setTransformationEpsilon(double trans_eps)
{
    ndt_.setTransformationEpsilon(trans_eps);
}

template <class PointSource, class PointTarget>
void LibNdtSlamPCLGPU<PointSource, PointTarget>::setStepSize(double step_size)
{
    ndt_.setStepSize(step_size);
}

template <class PointSource, class PointTarget>
void LibNdtSlamPCLGPU<PointSource, PointTarget>::setResolution(float res)
{
    ndt_.setResolution(res);
}

template <class PointSource, class PointTarget>
void LibNdtSlamPCLGPU<PointSource, PointTarget>::setMaximumIterations(int max_iter)
{
    ndt_.setMaximumIterations(max_iter);
}

template <class PointSource, class PointTarget>
double LibNdtSlamPCLGPU<PointSource, PointTarget>::getTransformationEpsilon()
{
    return ndt_.getTransformationEpsilon();
}

template <class PointSource, class PointTarget>
double LibNdtSlamPCLGPU<PointSource, PointTarget>::getStepSize() const
{
    return ndt_.getStepSize();
}

template <class PointSource, class PointTarget>
float LibNdtSlamPCLGPU<PointSource, PointTarget>::getResolution() const
{
    return ndt_.getResolution();
}

template <class PointSource, class PointTarget>
int LibNdtSlamPCLGPU<PointSource, PointTarget>::getMaximumIterations()
{
    return ndt_.getMaximumIterations();
}

template <class PointSource, class PointTarget>
void LibNdtSlamPCLGPU<PointSource, PointTarget>::align(const Pose& predict_pose)
{
    const auto predict_matrix = convertToEigenMatrix4f(predict_pose);
    ndt_.align(predict_matrix);
}

template <class PointSource, class PointTarget>
void LibNdtSlamPCLGPU<PointSource, PointTarget>::setInputTarget(const boost::shared_ptr< pcl::PointCloud<PointTarget> const>& map_ptr)
{
    boost::shared_ptr< pcl::PointCloud<PointTarget> > non_const_map_ptr(new pcl::PointCloud<PointTarget>(*map_ptr));
    ndt_.setInputTarget(non_const_map_ptr);
}

template <class PointSource, class PointTarget>
void LibNdtSlamPCLGPU<PointSource, PointTarget>::setInputSource(const boost::shared_ptr< pcl::PointCloud<PointSource> const>& scan_ptr)
{
    boost::shared_ptr< pcl::PointCloud<PointSource> > non_const_scan_ptr(new pcl::PointCloud<PointSource>(*scan_ptr));
    ndt_.setInputSource(non_const_scan_ptr);
}

template <class PointSource, class PointTarget>
double LibNdtSlamPCLGPU<PointSource, PointTarget>::getFitnessScore()
{
    return ndt_.getFitnessScore();
}

template <class PointSource, class PointTarget>
Pose LibNdtSlamPCLGPU<PointSource, PointTarget>::getFinalPose()
{
    return convertToPose(ndt_.getFinalTransformation());
}


#endif
