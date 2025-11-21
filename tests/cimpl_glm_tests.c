#include <stdbool.h>
#include <stdio.h>
#define CIMPL_IMPLEMENTATION
#include "cimpl_glm.h"

bool Quat_equal(Quat qa, Quat qb) {
    bool is_equal = equals_f32(qa.x, qb.x);
    is_equal &= equals_f32(qa.y, qb.y);
    is_equal &= equals_f32(qa.z, qb.z);
    is_equal &= equals_f32(qa.w, qb.w);
    return is_equal;
}

bool Mat4_equal(Mat4 ma, Mat4 mb) {
    bool is_equal = equals_f32(ma.xi, mb.xi);
    is_equal &= equals_f32(ma.xj, mb.xj);
    is_equal &= equals_f32(ma.xk, mb.xk);
    is_equal &= equals_f32(ma.xw, mb.xw);

    is_equal &= equals_f32(ma.yi, mb.yi);
    is_equal &= equals_f32(ma.yj, mb.yj);
    is_equal &= equals_f32(ma.yk, mb.yk);
    is_equal &= equals_f32(ma.yw, mb.yw);

    is_equal &= equals_f32(ma.zi, mb.zi);
    is_equal &= equals_f32(ma.zj, mb.zj);
    is_equal &= equals_f32(ma.zk, mb.zk);
    is_equal &= equals_f32(ma.zw, mb.zw);
    return is_equal;
}

bool Vec3_equal(Vec3 va, Vec3 vb) {
    bool is_equal = equals_f32(va.x, vb.x);
    is_equal &= equals_f32(va.y, vb.y);
    is_equal &= equals_f32(va.z, vb.z);
    return is_equal;
}

void test_quat_inverse(void) {
    Quat q = {0.5f, -0.3f, 0.7f, 0.2f};
    Quat q_inv = Quat_inverse(q);
    Quat q_identity = QUAT_IDENTITY;
    assert(
        Quat_equal(q_identity, Quat_mul(q, q_inv)) && "Quat inverse test failed"
    );
    printf("Quat inverse test succeeded.\n");
}

void test_quat_slerp(void) {
    // ~45° around X
    Quat qa = Quat_normalize((Quat){0.38f, 0.0f, 0.0f, 0.92f});
    // ~90° around Y
    Quat qb = Quat_normalize((Quat){0.0f, 0.707f, 0.0f, 0.707f});

    // t=0 should return qa
    assert(Quat_equal(qa, Quat_slerp(qa, qb, 0.0f)) && "Quat slerp t=0 failed");

    // t=1 should return qb
    assert(Quat_equal(qb, Quat_slerp(qa, qb, 1.0f)) && "Quat slerp t=1 failed");

    // t=0.5: verify result is normalized and equidistant
    Quat qm = Quat_slerp(qa, qb, 0.5f);
    assert(
        equals_f32(Quat_length(qm), 1.0f) &&
        "Quat slerp midpoint not normalized"
    );

    // Midpoint should be equidistant from both (equal dot products)
    f32 dot_a = Quat_dot(qa, qm);
    f32 dot_b = Quat_dot(qb, qm);
    assert(equals_f32(dot_a, dot_b) && "Quat slerp midpoint not equidistant");

    // Hemisphere flip: qc represents same rotation as qb but negated
    // slerp should still produce valid interpolation (take short path)
    Quat qc = (Quat){-qb.x, -qb.y, -qb.z, -qb.w};
    Quat qm_flip = Quat_slerp(qa, qc, 0.5f);
    assert(
        equals_f32(Quat_length(qm_flip), 1.0f) &&
        "Quat slerp hemisphere flip not normalized"
    );
    // Result should be equivalent to normal slerp (same rotation)
    // Either equal or negated (both represent same rotation)
    bool same = Quat_equal(qm, qm_flip);
    Quat qm_flip_neg = (Quat){-qm_flip.x, -qm_flip.y, -qm_flip.z, -qm_flip.w};
    bool same_neg = Quat_equal(qm, qm_flip_neg);
    assert(
        (same || same_neg) &&
        "Quat slerp hemisphere flip produced different rotation"
    );
    printf("Quat slerp test succeeded.\n");
}

void test_quat_euler_roundtrip(void) {
    // Arbitrary euler angles (radians): roll=30°, pitch=45°, yaw=60°
    Vec3 euler = {0.524f, 0.785f, 1.047f};

    Quat q = Quat_from_euler(euler);
    Vec3 euler_back = Quat_to_euler(q);

    assert(Vec3_equal(euler, euler_back) && "Quat/Euler roundtrip failed");

    // Test near-zero angles
    Vec3 euler_small = {0.1f, 0.05f, 0.15f};
    Quat q_small = Quat_from_euler(euler_small);
    Vec3 euler_small_back = Quat_to_euler(q_small);

    assert(
        Vec3_equal(euler_small, euler_small_back) &&
        "Quat/Euler small roundtrip failed"
    );
    printf("Quat/Euler conversion test succeeded.\n");
}

void test_quat_mat3_roundtrip(void) {
    // Arbitrary normalized quaternion
    Quat q = {0.5f, -0.3f, 0.7f, 0.2f};
    q = Quat_normalize(q);

    Mat3 m = Mat3_from_quat(q);
    Quat q_back = Quat_from_mat3(m);
    q_back = Quat_normalize(q_back);

    // q and -q represent same rotation
    Quat q_neg = {-q.x, -q.y, -q.z, -q.w};
    bool match = Quat_equal(q, q_back) || Quat_equal(q_neg, q_back);
    assert(match && "Quat/Mat3 roundtrip failed");

    // Test identity
    Quat q_id = QUAT_IDENTITY;
    Mat3 m_id = Mat3_from_quat(q_id);
    Quat q_id_back = Quat_normalize(Quat_from_mat3(m_id));
    Quat q_id_neg = {-q_id.x, -q_id.y, -q_id.z, -q_id.w};
    bool match_id =
        Quat_equal(q_id, q_id_back) || Quat_equal(q_id_neg, q_id_back);
    assert(match_id && "Quat/Mat3 roundtrip identity failed");

    // Test axis-aligned rotation (90° around Z)
    Quat q_z90 = Quat_normalize((Quat){0.0f, 0.0f, 0.707f, 0.707f});
    Mat3 m_z90 = Mat3_from_quat(q_z90);
    Quat q_z90_back = Quat_normalize(Quat_from_mat3(m_z90));
    Quat q_z90_neg = (Quat){-q_z90.x, -q_z90.y, -q_z90.z, -q_z90.w};
    bool match_z90 =
        Quat_equal(q_z90, q_z90_back) || Quat_equal(q_z90_neg, q_z90_back);
    assert(match_z90 && "Quat/Mat3 roundtrip z90 failed");
    printf("Quat/Mat3 conversion test succeeded.\n");
}

void test_mat4_inverse_rigid(void) {
    // Create a rigid transform: rotation + translation
    Quat q = Quat_normalize((Quat){0.5f, -0.3f, 0.7f, 0.2f});
    Vec3 t = {3.5f, -2.0f, 7.0f};
    Mat4 m = Mat4_from_translation_quat(t, q);

    Mat4 m_inv = Mat4_inverse_rigid(m);
    Mat4 identity = MAT4_IDENTITY;

    // M * M^-1 should equal identity
    Mat4 result = Mat4_mul(m, m_inv);
    assert(Mat4_equal(identity, result) && "Mat4 rigid inverse M*M_inv failed");

    // M^-1 * M should also equal identity
    Mat4 result2 = Mat4_mul(m_inv, m);
    assert(
        Mat4_equal(identity, result2) && "Mat4 rigid inverse M_inv*M failed"
    );

    // Test with pure rotation (no translation)
    Vec3 t_zero = VEC3_ZERO;
    Mat4 m_rot = Mat4_from_translation_quat(t_zero, q);
    Mat4 m_rot_inv = Mat4_inverse_rigid(m_rot);
    Mat4 result_rot = Mat4_mul(m_rot, m_rot_inv);
    assert(
        Mat4_equal(identity, result_rot) &&
        "Mat4 rigid inverse pure rotation failed"
    );

    // Test with pure translation (no rotation)
    Quat q_id = QUAT_IDENTITY;
    Mat4 m_trans = Mat4_from_translation_quat(t, q_id);
    Mat4 m_trans_inv = Mat4_inverse_rigid(m_trans);
    Mat4 result_trans = Mat4_mul(m_trans, m_trans_inv);
    assert(
        Mat4_equal(identity, result_trans) &&
        "Mat4 rigid inverse pure translation failed"
    );
    printf("Mat4 rigid inverse test succeeded.\n");
}

void test_mat4_orthonormalize(void) {
    // Create a slightly non-orthonormal matrix (simulate drift)
    Mat4 m = MAT4_IDENTITY;
    // clang-format off
    m.xi = 1.01f; m.xj = 0.02f; m.xk = 0.0f;
    m.yi = 0.01f; m.yj = 0.99f; m.yk = 0.03f;
    m.zi = 0.0f; m.zj = 0.02f; m.zk = 1.02f;
    m.ti = 5.0f; m.tj = 3.0f; m.tk = -2.0f;
    // clang-format on

    Mat4 m_orth = Mat4_orthonormalize(m);

    // Extract basis vectors
    Vec3 x = {m_orth.xi, m_orth.xj, m_orth.xk};
    Vec3 y = {m_orth.yi, m_orth.yj, m_orth.yk};
    Vec3 z = {m_orth.zi, m_orth.zj, m_orth.zk};

    // Check unit length
    assert(
        equals_f32(Vec3_length(x), 1.0f) &&
        "Mat4 orthonormalize x-axis not unity"
    );
    assert(
        equals_f32(Vec3_length(y), 1.0f) &&
        "Mat4 orthonormalize y-axis not unity"
    );
    assert(
        equals_f32(Vec3_length(z), 1.0f) &&
        "Mat4 orthonormalize z-axis not unity"
    );

    // Check orthogonality (dot products should be 0)
    assert(
        equals_f32(Vec3_dot(x, y), 0.0f) &&
        "Mat4 orthonormalize x & y axes not orthogonal"
    );
    assert(
        equals_f32(Vec3_dot(x, z), 0.0f) &&
        "Mat4 orthonormalize x & z axes not orthogonal"
    );
    assert(
        equals_f32(Vec3_dot(y, z), 0.0f) &&
        "Mat4 orthonormalize y & z axes not orthogonal"
    );

    // Check translation preserved
    assert(
        equals_f32(m_orth.ti, m.ti) &&
        "Mat4 orthonormalize translation x changed"
    );
    assert(
        equals_f32(m_orth.tj, m.tj) &&
        "Mat4 orthonormalize translation y changed"
    );
    assert(
        equals_f32(m_orth.tk, m.tk) &&
        "Mat4 orthonormalize translation z changed"
    );
    printf("Mat4 orthonormalize test succeeded.\n");
}

int main(void) {
    test_quat_inverse();
    test_quat_slerp();
    test_quat_euler_roundtrip();
    test_quat_mat3_roundtrip();
    test_mat4_inverse_rigid();
    test_mat4_orthonormalize();
    return 0;
}
