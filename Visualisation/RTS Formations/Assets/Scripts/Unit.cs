using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Unit : MonoBehaviour
{

    [SerializeField] Camera _camera;
    private Vector3 _targetPosition;
    private float _speed = 2.0f;



    private void Start()
    {

    }
    // Update is called once per frame
    void Update()
    {
       // Vector3 mousePos = Input.mousePosition;
       //// mousePos.z = _camera.nearClipPlane;
       // _targetPosition = _camera.ScreenToWorldPoint(mousePos);
       // _targetPosition.y = 0.0f;
       // Debug.Log(_targetPosition);

       // //float step = Time.fixedDeltaTime * _speed;
       // //transform.position = Vector3.MoveTowards(transform.position, _targetPosition, step);

        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        if(Physics.Raycast(ray,out RaycastHit hit))
        {
            Debug.Log(hit.point);
            Vector3 targetPos = hit.point;
            targetPos.y = 0.0f;
            float step = Time.fixedDeltaTime * _speed;
            transform.position = Vector3.MoveTowards(transform.position, targetPos, step);
        }

        // Debug.Log(ray.GetPoint(distance));
    }
}
