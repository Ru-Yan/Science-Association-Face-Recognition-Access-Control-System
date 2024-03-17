using System;
using System.Linq;
using Facecore.Models;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace Facecore.Controllers
{
    [Route("api/[controller]/[action]")]
    [ApiController]
    public class ReserveController : Controller
    {
        private readonly ILogger<UserController> _logger;
        private readonly FacecoreContext _context;

        public ReserveController(ILogger<UserController> logger, FacecoreContext context)
        {
            _logger = logger;
            _context = context;
        }

        public object getAllReserve()
        {
            try
            {
                var reserve = _context.reserve;
                return reserve;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return StatusCode(404);
            }

        }

        public object getReserveByUID(string uid)
        {
            try
            {
                var reserve = _context.reserve.Where(e=>e.stu_id==uid);
                return reserve;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return StatusCode(404);
            }

        }

        public object getReserveByLibID(string lid)
        {
            try
            {
                var reserve = _context.reserve.Where(e=>e.lib_id==lid);
                return reserve;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return StatusCode(404);
            }

        }

        public object setStatue(int id,string signIn = null,string signOut = null)
        {
            try
            {
                var reserve = _context.reserve.First(e=>e.Id==id);

                if (signIn != null)
                {
                    reserve.come_state = signIn;
                }

                if (signOut != null)
                {
                    reserve.leave_state = signOut;
                }

                _context.SaveChanges();

                return new
                {
                    res = "succ",
                    description = "成功更新",
                    reserve.come_state,
                    reserve.leave_state
                };
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return new
                {
                    res = "err",
                    description = "未找到记录或遇到其他错误",
                    errms = e
                };
            }
        }

        [HttpGet]
        public IActionResult Home()
        {
            ViewBag.lib = _context.lib;
            return View();
        }

        [HttpPost]
        public IActionResult reserve([FromForm]reserve model,string msg = "",string err = "")
        {
            ViewBag.lib = _context.lib;
            try
            {
                var stu = _context.stu.First(e => e.stu_id == model.stu_id);
                var lib = _context.lib.First(e => e.lib_id == model.lib_id);
                model.stu_name = stu.stu_name;
                model.stu_sex = stu.stu_sex;
                model.lib_name = lib.lib_name;
                _context.reserve.Add(model);
                _context.SaveChanges();
                ViewBag.msg = "预约成功："+model.stu_name+"，您的预约号为" + model.Id + "您可以到馆刷脸通过";
                return View("Home");
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                ViewBag.err = "未找到您的信息";
                return View("Home");
            }
        }
    }
}
