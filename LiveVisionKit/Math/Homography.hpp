//    *************************** LiveVisionKit ****************************
//    Copyright (C) 2022  Sebastian Di Marco (crowsinc.dev@gmail.com)
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.
// 	  **********************************************************************

#pragma once

#include <opencv2/opencv.hpp>

namespace lvk
{

	class Homography
	{
	public:

        struct EstimationParams
        {
            int method = cv::RANSAC;
            double confidence = 0.99;
            double error_threshold = 4;
            size_t max_iterations = 2000;
            size_t refine_iterations = 10;
        };

        static std::optional<Homography> Estimate(
            const std::vector<cv::Point2f>& tracked_points,
            const std::vector<cv::Point2f>& matched_points,
            std::vector<uint8_t>& inlier_status,
            EstimationParams sampling_method,
            bool force_rigid_affine = false
        );

        static std::optional<Homography> Estimate(
            const std::vector<cv::Point2f>& tracked_points,
            const std::vector<cv::Point2f>& matched_points,
            std::vector<uint8_t>& inlier_status,
            cv::UsacParams sampling_method
        );


		static Homography Zero();

		static Homography Identity();

        static Homography WrapMatrix(cv::Mat& matrix);

        static Homography FromAffineMatrix(const cv::Mat& affine);


		Homography();

		explicit Homography(const cv::Mat& matrix);

        Homography(Homography&& other) noexcept;

        Homography(const Homography& other);

		cv::Point2d transform(const cv::Point2d& point) const;

        cv::Point2d operator*(const cv::Point2d& point) const;

		cv::Point2f transform(const cv::Point2f& point) const;

        cv::Point2f operator*(const cv::Point2f& point) const;

		std::vector<cv::Point2d> transform(const std::vector<cv::Point2d>& points) const;

        std::vector<cv::Point2d> operator*(const std::vector<cv::Point2d>& points) const;

		std::vector<cv::Point2f> transform(const std::vector<cv::Point2f>& points) const;

        std::vector<cv::Point2f> operator*(const std::vector<cv::Point2f>& points) const;

		void warp(const cv::UMat& src, cv::UMat& dst) const;

		cv::Mat as_matrix() const;

		bool is_affine() const;

        Homography& operator=(const cv::Mat& other);

        Homography& operator=(const Homography& other);

        Homography& operator=(Homography&& other) noexcept;

		void operator+=(const Homography& other);

        void operator+=(const cv::Mat& other);

        void operator-=(const Homography& other);

        void operator-=(const cv::Mat& other);

        void operator*=(const Homography& other);

        void operator*=(const cv::Mat& other);

		void operator*=(const double scaling);

		void operator/=(const double scaling);


	private:
        explicit Homography(cv::Mat& data);

		cv::Mat m_Matrix;
	};

	Homography operator+(const Homography& left, const Homography& right);

	Homography operator-(const Homography& left, const Homography& right);

	Homography operator*(const Homography& left, const Homography& right);

	Homography operator*(const Homography& homography, const double scaling);

	Homography operator/(const Homography& homography, const double scaling);

	Homography operator*(const double scaling, const Homography& homography);

	Homography operator/(const double scaling, const Homography& homography);

}
