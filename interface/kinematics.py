"""Inverse kinematics of robotic arm"""
import math

def inverse_kinematics(x, y, z):
    """Inverse kinematics of robotic arm"""
    base_angle = math.atan2(y, x)
    chord_sq = x**2 + y**2 + z**2
    link_angle = math.acos((128 - chord_sq) / 128)
    planar_base = math.atan2(z, math.sqrt(x**2 + y**2))
    planar_base += (math.pi - link_angle) / 2
    link_angle += planar_base
    return (math.degrees(base_angle), math.degrees(planar_base), math.degrees(link_angle))
