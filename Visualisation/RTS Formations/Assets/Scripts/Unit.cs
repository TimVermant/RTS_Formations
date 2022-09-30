using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Unit : MonoBehaviour
{

    [SerializeField] Camera _camera;

    [SerializeField] private Material _unitMaterial;
    [SerializeField] private Material _leaderMaterial;

    private Vector3 _targetPosition;
    public Vector3 TargetPosition
    {
        get { return _targetPosition; }
        set { _targetPosition = value; }
    }
    private float _speed = 2.0f;

    private bool _isLeader = false;


    private MeshRenderer _meshRenderer;


    private void Start()
    {
        _camera = FindObjectOfType<Camera>();
        _meshRenderer = GetComponent<MeshRenderer>();
    }

    private void Update()
    {
        if(_isLeader)
        {
            float step = Time.fixedDeltaTime * _speed;
            transform.position = Vector3.MoveTowards(transform.position, _targetPosition, step);
        }
    }

    public void MakeLeader(Vector3 startPos)
    {
        _meshRenderer.sharedMaterial = _leaderMaterial;
        _isLeader = true;
        _targetPosition = startPos;
    }

    public void ResetUnit()
    {
        _meshRenderer.sharedMaterial = _unitMaterial;
        _isLeader = false;
    }

}
